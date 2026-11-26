#include "SnakeGame.h"

#include "../Config.h"

#include "sPlayer.h"
#include "Apple.h"

namespace Snake {

	SnakeGame::SnakeGame(GameContext& ctx) : _input(&ctx.input) {
		SpawnBounds();
		SpawnBarriers();
		SpawnAliens();

		auto player = std::make_unique<sPlayer>(ctx.input, *this, SpaceInvaders::PlayMinX, static_cast<float>(SpaceInvaders::PlayerY));
		_player = player.get();                 // observe it...
		_objects.push_back(std::move(player));  // ...then hand ownership to the vector
	}

	void SnakeGame::SpawnBounds() {

	}

	void SnakeGame::SpawnAliens() {

	}

	void SnakeGame::SpawnBarriers() {

	}

	void SnakeGame::Update(float deltaTime) {
		if (_input->IsKeyPressed(VK_ESCAPE)) { _isFinished = true; return; }

		for (auto& obj : _objects) obj->Update(deltaTime);

		for (auto& obj : _pending) _objects.push_back(std::move(obj));
		_pending.clear();

		if (!_gameOver) {

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

		//Remove dead aliens and objects
		//Pruning pointers before the object is gone
		//_aliens.erase(
		//	std::remove_if(_aliens.begin(), _aliens.end(),
		//		[](const Alien* a) { return !a->IsAlive(); }),
		//	_aliens.end());

		//_objects.erase(
		//    std::remove_if(_objects.begin(), _objects.end(),
		//        [](const std::unique_ptr<GameObject>& o) { return !o->IsAlive(); }),
		//    _objects.end());

		//wincheck
		if (_player && _player->IsAlive()) {

		}
	}

	void SnakeGame::Draw(Console& console) const {
		for (const auto& obj : _objects) obj->Draw(console);

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

	void SnakeGame::SpawnApple(float x, float y) {

	}
}
