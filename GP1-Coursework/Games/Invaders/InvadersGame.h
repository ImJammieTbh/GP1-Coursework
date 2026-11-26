#pragma once
#include "../IGame.h"
#include "../../Engine/GameContext.h"
#include "../../Engine/GameObject.h"
#include "Alien.h"
#include <memory>
#include <vector>
#include <random>

namespace SpaceInvaders {
	class Player;
	class Missile;

	class InvadersGame : public IGame {
	public:
		explicit InvadersGame(GameContext& ctx);

		void Update(float deltaTime) override;
		void Draw(Console& console) const override;
		bool IsFinished() const override { return _isFinished; }

		void SpawnMissile(float x, float y);

	private:
		std::vector<std::unique_ptr<GameObject>> _pending;

		void SpawnBounds();

		void SpawnAliens();
		void SpawnBarriers();

		void StepAliens();
		bool AliensAtBottom() const;

		Input* _input;
		Player* _player = nullptr;
		std::vector<std::unique_ptr<GameObject>> _objects;
		bool _isFinished = false;

		std::vector<Alien*> _aliens;
		float _moveTimer = 0.0f;
		float _moveDelay = 1.0f;
		float _minMoveDelay = 0.1f;   // speed cap so they don't become unplayable
		float _moveDirection = 1.0f; //1.0f = right, -1.0f = left
		bool _gameOver = false;
		bool _playerWon = false;

		Missile* _missile = nullptr;

		void AlienShoot();
		float _bombTimer = 0.0f;
		float _bombDelay = 1.5f;     // seconds between shots
		std::mt19937 _rng{ std::random_device{}() };
	};
}
