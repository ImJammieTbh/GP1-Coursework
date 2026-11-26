#pragma once
#include "../../Engine/GameObject.h"

namespace SpaceInvaders {
	class Missile : public GameObject {
	public:
		Missile(float x, float y) : GameObject(x, y, 1, 1, L'|', Colour::LightYellow) {}

		void Update(float deltaTime) override;

		float moveInterval = 0.1f; //seconds between moves
		void MoveUp() { SetPosition(X(), Y() - 1); }

		Layer GetLayer() const override { return Layer::PlayerMissile; }
		void OnHit(GameObject& other) override {
			Layer l = other.GetLayer();
			if (l == Layer::Alien || l == Layer::Barrier) Kill();
		}
	private:
		float _timer = 0.0f;
	};
}
