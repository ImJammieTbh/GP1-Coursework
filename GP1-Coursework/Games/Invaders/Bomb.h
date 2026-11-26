#pragma once
#include "../../Engine/GameObject.h"
#include "../Config.h"

namespace SpaceInvaders {
	class Bomb : public GameObject {
	public:
		Bomb(float x, float y) : GameObject(x, y, 1, 1, L'v', Colour::Red) {}

		void Update(float deltaTime) override;

		Layer GetLayer() const override { return Layer::AlienBomb; }
		void OnHit(GameObject& other) override {
			Layer l = other.GetLayer();
			if (l == Layer::Player || l == Layer::Barrier) Kill();
		}

		float moveInterval = 0.15f;

	private:
		float _timer = 0.0f;
	};
}
