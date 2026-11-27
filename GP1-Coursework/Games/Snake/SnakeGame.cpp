#include "SnakeGame.h"

#include "../Config.h"

#include "sPlayer.h"
#include "Apple.h"
#include "sPlayerTail.h"
#include "Bounds.h"

#include <random>
#include <vector>

namespace Snake {

	SnakeGame::SnakeGame(GameContext& ctx) : _input(&ctx.input) {
		SpawnBounds();
		SpawnHorizontalBounds();

		auto player = std::make_unique<sPlayer>(ctx.input, *this, Snake::PlayMinX, static_cast<float>(Snake::PlayerY));
		_player = player.get();                 // observe it...
		_objects.push_back(std::move(player));  // ...then hand ownership to the vector
	}

	void SnakeGame::SpawnBounds() {
		for (int y = BoundsTopY; y <= BoundsBottomY; ++y) {
			for (int t = 0; t < BoundsThickness; ++t) {
				_objects.push_back(std::make_unique<Bounds>(
					static_cast<float>(BoundsLeftX + t), static_cast<float>(y)));
				_objects.push_back(std::make_unique<Bounds>(
					static_cast<float>(BoundsRightX + t), static_cast<float>(y)));
			}
		}
	}

	void SnakeGame::SpawnHorizontalBounds() {
		for (int x = BoundsLeftX; x <= BoundsRightX; ++x) {
			_objects.push_back(std::make_unique<Bounds>(
				static_cast<float>(x), static_cast<float>(BoundsTopY)));
			_objects.push_back(std::make_unique<Bounds>(
				static_cast<float>(x), static_cast<float>(BoundsBottomY)));
		}
	}

	void SnakeGame::Update(float deltaTime) {
		if (_input->IsKeyPressed(VK_ESCAPE)) { _isFinished = true; return; }

		for (auto& obj : _objects) obj->Update(deltaTime);

		for (auto& obj : _pending) _objects.push_back(std::move(obj));
		_pending.clear();


		for (size_t i = 0; i < _objects.size(); ++i) {
			for (size_t j = i + 1; j < _objects.size(); ++j) {
				GameObject& a = *_objects[i];
				GameObject& b = *_objects[j];
				if (!a.IsAlive() || !b.IsAlive()) continue;
				if (Overlaps(a.Bounds(), b.Bounds())) {
					a.OnHit(b);
					b.OnHit(a);
				}
			}
		}

		if (_player && !_player->IsAlive()) { _player = nullptr; _gameOver = true; }

		// Check whether fruit has been hit
		if (_apple && !_apple->IsAlive()) {
			_apple = nullptr;
		}
		
		//Remove dead aliens and objects
		//Pruning pointers before the object is gone
		_tailSegments.erase(
			std::remove_if(_tailSegments.begin(), _tailSegments.end(),
				[](const sPlayerTail* t) { return !t->IsAlive(); }),
			_tailSegments.end());

		_objects.erase(
		    std::remove_if(_objects.begin(), _objects.end(),
		        [](const std::unique_ptr<GameObject>& o) { return !o->IsAlive(); }),
		    _objects.end());

		if (!_apple && !_gameOver) {
			SpawnApple();
		}

		//wincheck
		if (_maxTailLength >= 25) {
			_playerWon = true;
		}
	}

	void SnakeGame::Draw(Console& console) const {
		for (const auto& obj : _objects) obj->Draw(console);

		console.DrawWrappedString(0, 0, L"Use arrow keys to \nmove. \nEat the fruit to \ngrow your tail. \nAvoid hitting walls or your own tail.", 20, 7);

		if (_gameOver) {
			const wchar_t* msg = L"GAME OVER - ESC to return";
			for (int i = 0; msg[i] != L'\0'; ++i)
				console.Draw(static_cast<short>(27 + i), 12, msg[i], 12);
		}

		if (_playerWon) {
			const wchar_t* msg = L"YOU WIN - ESC to return";
			for (int i = 0; msg[i] != L'\0'; ++i)
				console.Draw(static_cast<short>(27 + i), 12, msg[i], 10);
		}
	}

	void SnakeGame::SpawnApple() {
		if (_apple) return;

		std::vector<std::pair<int, int>> freePositions;

		// Check every playable grid position
		for (int x = PlayMinX; x < PlayMaxX; ++x) {
			for (int y = BoundsTopY + 1; y < GroundY; ++y) {

				Apple candidate(static_cast<float>(x), static_cast<float>(y));

				bool occupied = false;

				// Check existing objects
				for (const auto& obj : _objects) {
					if (obj->IsAlive() &&
						Overlaps(candidate.Bounds(), obj->Bounds())) {
						occupied = true;
						break;
					}
				}

				// Check objects waiting to be added
				if (!occupied) {
					for (const auto& obj : _pending) {
						if (obj->IsAlive() &&
							Overlaps(candidate.Bounds(), obj->Bounds())) {
							occupied = true;
							break;
						}
					}
				}

				if (!occupied) {
					freePositions.push_back({ x, y });
				}
			}
		}

		// No available positions and WIN 
		if (freePositions.empty()) {
			_playerWon = true;
			return;
		}

		// Pick a random free position
		static std::mt19937 rng(std::random_device{}());

		std::uniform_int_distribution<size_t> dist(
			0, freePositions.size() - 1
		);

		auto [x, y] = freePositions[dist(rng)];

		auto apple = std::make_unique<Apple>(
			static_cast<float>(x),
			static_cast<float>(y)
		);

		_apple = apple.get();
		_pending.push_back(std::move(apple));
	}

	void SnakeGame::SpawnTail(float x, float y) {
		if (_maxTailLength <= 0) return;
		
		if (!_tailSegments.empty() && _tailSegments.size() >= _maxTailLength) {
			_tailSegments.front()->Kill();
			_tailSegments.erase(_tailSegments.begin());
		}

		auto tailSegment = std::make_unique<sPlayerTail>(x, y);
		_tailSegments.push_back(tailSegment.get());
		_pending.push_back(std::move(tailSegment));
	}

	bool SnakeGame::isPositionOccupied(float x, float y) const {
		GameObject* player = nullptr;

		for (const auto& obj : _objects) {
			if (obj->GetLayer() == Layer::Player && obj->IsAlive()) {
				player = obj.get();
				break;
			}
		}

		if (!player) return true;

		for (const auto& obj : _objects) {
			if (!obj->IsAlive() && obj.get() == player) continue;
		}
		return false;
	}

	bool SnakeGame::isWallAhead(float x, float y) const {
		Rect nextPosition{ x, y, 1.0f, 1.0f };

		for (const auto& obj : _objects) {
			if (!obj->IsAlive()) continue;

			if (obj->GetLayer() == Layer::Bounds) {
				if (Overlaps(nextPosition, obj->Bounds())) {
					return true;
				}
			}
		}
		return false;
	}
}