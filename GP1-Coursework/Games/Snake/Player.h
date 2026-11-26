#pragma once
#include "../../Engine/GameObject.h"
#include "../../Engine/Input.h"

class InvadersGame;

class Player : public GameObject {
public:
	Player(Input& input, InvadersGame& game, float x, float y);
	void Update(float deltaTime) override;

	Layer GetLayer() const override { return Layer::Player; }
	void OnHit(GameObject& other) override {
		if (other.GetLayer() == Layer::AlienBomb) Kill();   // swap for lives/health later
	}

private:
	Input* _input;
	InvadersGame* _game;
	static constexpr float speed = 20.0f; //cps
};