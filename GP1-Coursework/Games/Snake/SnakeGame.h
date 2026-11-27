#pragma once
#include "../IGame.h"
#include "../../Engine/GameContext.h"
#include "../../Engine/GameObject.h"
#include <memory>
#include <vector>

#include "sPlayer.h"
#include "sPlayerTail.h"

namespace Snake {
	class Apple;

	class SnakeGame : public IGame {
	public:
		explicit SnakeGame(GameContext& ctx);

		void Update(float deltaTime) override;
		void Draw(Console& console) const override;
		bool IsFinished() const override { return _isFinished; }

		void SpawnMissile(float x, float y);

		int _maxTailLength = 0;
		void SpawnTail(float x, float y);

		bool isGameOver() const { return _gameOver; }
		bool isPlayerWon() const { return _playerWon; }

		bool isPositionOccupied(float x, float y) const;
		bool isWallAhead(float x, float y) const;

		void GameOver() { _gameOver = true; }

	private:
		std::vector<std::unique_ptr<GameObject>> _pending;

		void SpawnBounds();
		void SpawnHorizontalBounds();

		void SpawnApple();

		Input* _input;
		sPlayer* _player = nullptr;
		std::vector<std::unique_ptr<GameObject>> _objects;
		bool _isFinished = false;

		bool _gameOver = false;
		bool _playerWon = false;

		Apple* _apple = nullptr;

		std::vector<sPlayerTail*> _tailSegments;
	};
}
