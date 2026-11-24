#pragma once
#include "../../Engine/GameObject.h"

class Alien : public GameObject {
public:
	Alien(float x, float y) : GameObject(x, y, 1, 1, L'A', Colour::LightGreen) {}
	void Move(float dx, float dy) { SetPosition(X() + dx, Y() + dy); }

	Layer GetLayer() const override { return Layer::Alien; }
	void OnHit(GameObject& other) override {
		if (other.GetLayer() == Layer::PlayerMissile) Kill();
	}
};