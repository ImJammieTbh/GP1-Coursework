#include "InvadersGame.h"

#include "../Config.h"
#include "Bounds.h"
#include "Alien.h"
#include "Barrier.h"
#include "Ground.h"
#include "Player.h"
#include "Bomb.h"

#include <algorithm>
#include <map>
#include "Missile.h"

namespace SpaceInvaders {

	InvadersGame::InvadersGame(GameContext& ctx) : _input(&ctx.input) {
		_objects.push_back(std::make_unique<Ground>());
		SpawnBounds();
		SpawnBarriers();
		SpawnAliens();

		auto player = std::make_unique<Player>(ctx.input, *this, PlayMinX, static_cast<float>(PlayerY));
		_player = player.get();                 // observe it...
		_objects.push_back(std::move(player));  // ...then hand ownership to the vector
	}

	void InvadersGame::SpawnBounds() {
		for (int y = BoundsTopY; y <= BoundsBottomY; ++y) {
			for (int t = 0; t < BoundsThickness; ++t) {
				_objects.push_back(std::make_unique<Bounds>(
					static_cast<float>(BoundsLeftX + t), static_cast<float>(y)));
				_objects.push_back(std::make_unique<Bounds>(
					static_cast<float>(BoundsRightX + t), static_cast<float>(y)));
			}
		}
	}

	void InvadersGame::SpawnAliens() {
		for (int row = 0; row < alienRows; ++row) {
			for (int col = 0; col < alienCols; ++col) {
				auto alien = std::make_unique<Alien>(
					static_cast<float>(alienStartX + col * alienSpacingX),
					static_cast<float>(alienStartY + row * alienSpacingY));
				_aliens.push_back(alien.get());          //observe it
				_objects.push_back(std::move(alien));    //then hand ownership to the vector
			}
		}
	}

	void InvadersGame::SpawnBarriers() {
		for (int group = 0; group < barrierGroupCount; ++group) {
			const int groupX = barrierX + group * (barrierWidth + barrierGaps);
			for (int row = 0; row < barrierHeight; ++row) {
				for (int col = 0; col < barrierWidth; ++col) {
					_objects.push_back(std::make_unique<Barrier>(
						static_cast<float>(groupX + col),
						static_cast<float>(barrierY + row)));
				}
			}
		}
	}

	void InvadersGame::Update(float deltaTime) {
		if (_input->IsKeyPressed(VK_ESCAPE)) { _isFinished = true; return; }

		for (auto& obj : _objects) obj->Update(deltaTime);

		_bombTimer += deltaTime;
		if (!_gameOver && _bombTimer >= _bombDelay) {
			_bombTimer -= _bombDelay;
			AlienShoot();
		}

		for (auto& obj : _pending) _objects.push_back(std::move(obj));
		_pending.clear();

		if (!_gameOver) {
			_moveTimer += deltaTime;
			if (_moveTimer >= _moveDelay) {
				_moveTimer -= _moveDelay;
				StepAliens();
				if (AliensAtBottom()) {
					_gameOver = true;
					//_isFinished = true;
				}
			}
		}

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
		if (_missile && !_missile->IsAlive()) _missile = nullptr;

		//Remove dead aliens and objects
		//Pruning pointers before the object is gone
		_aliens.erase(
			std::remove_if(_aliens.begin(), _aliens.end(),
				[](const Alien* a) { return !a->IsAlive(); }),
			_aliens.end());

		_objects.erase(
			std::remove_if(_objects.begin(), _objects.end(),
				[](const std::unique_ptr<GameObject>& o) { return !o->IsAlive(); }),
			_objects.end());

		const float t = static_cast<float>(_aliens.size()) / (alienRows * alienCols);
		_moveDelay = _minMoveDelay + t * (1.0f - _minMoveDelay);   // 1.0f = starting delay

		//wincheck
		if (_player && _player->IsAlive()) {
			bool allDead = true;
			for (const Alien* a : _aliens) {
				if (a->IsAlive()) { allDead = false; break; }
			}
			if (allDead) _playerWon = true;
		}
	}

	void InvadersGame::Draw(Console& console) const {
		for (const auto& obj : _objects) obj->Draw(console);

		console.DrawWrappedString(0, 0, L"Use arrow keys to \nmove, press SPACE toshoot. \nShoot the aliens to win. \nDont let the aliens reach the bottom.", 20, 7);

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

	void InvadersGame::StepAliens() {
		bool hitEdge = false;
		for (const Alien* a : _aliens) {
			if (!a->IsAlive()) continue;
			const float nextX = a->X() + _moveDirection;
			if (nextX < PlayMinX || nextX + 1.0f > PlayMaxX) { hitEdge = true; break; }
		}

		for (Alien* a : _aliens) {
			if (!a->IsAlive()) continue;
			if (hitEdge) a->Move(0.0f, 1.0f);
			else         a->Move(_moveDirection, 0.0f);
		}
		if (hitEdge) {
			_moveDirection = -_moveDirection;
			_moveDelay = std::max(_moveDelay * 0.9f, _minMoveDelay);   // 10% faster each bounce
		}
	}

	bool InvadersGame::AliensAtBottom() const {
		for (const Alien* a : _aliens) {
			if (a->IsAlive() && a->Y() + 1.0f >= PlayerY) return true;
		}
		return false;
	}

	void InvadersGame::SpawnMissile(float x, float y) {
		if (_missile) return;
		auto missile = std::make_unique<Missile>(x, y);
		_missile = missile.get();
		_pending.push_back(std::move(missile));
	}

	void InvadersGame::AlienShoot() {
		std::map<int, const Alien*> lowestInColumn;
		for (const Alien* a : _aliens) {
			if (!a->IsAlive()) continue;
			const int col = static_cast<int>(a->X());
			auto it = lowestInColumn.find(col);
			if (it == lowestInColumn.end() || a->Y() > it->second->Y()) {
				lowestInColumn[col] = a;
			}
		}
		if (lowestInColumn.empty()) return;

		std::vector<const Alien*> possibleShooters;
		possibleShooters.reserve(lowestInColumn.size());
		for (const auto& [col, alien] : lowestInColumn) {
			possibleShooters.push_back(alien);
		}

		std::uniform_int_distribution<size_t> pick(0, possibleShooters.size() - 1);
		const Alien* shooter = possibleShooters[pick(_rng)];
		_pending.push_back(std::make_unique<Bomb>(shooter->X(), shooter->Y() + 1));
	}

}
