#pragma once
#include "../../Engine/GameObject.h"
#include "../../Engine/Input.h"

namespace Snake {
	class SnakeGame;

	class sPlayer : public GameObject {
	public:
		sPlayer(Input& input, SnakeGame& game, float x, float y);
		void Update(float deltaTime) override;
		void OnHit(GameObject& other) override;

		Layer GetLayer() const override { return Layer::Player; }

	private:
		Input* _input;
		SnakeGame* _game;
		
		static constexpr float moveInterval = 0.15f;
		float _moveTimer = 0.0f;

		int _dirX = 0, _dirY = 0;
		int _nextXdir = 0, _nextYdir = 0;
	};
}
