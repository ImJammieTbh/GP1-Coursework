#pragma once
#include "../../Engine/GameObject.h"

namespace SpaceInvaders {
	class Barrier : public GameObject {
	public:
		Barrier(float x, float y) : GameObject(x, y, 1, 1, 9608, Colour::LightGrey) {}

		Layer GetLayer() const override { return Layer::Barrier; }
		void OnHit(GameObject& other) override {
			Layer l = other.GetLayer();
			if (l == Layer::PlayerMissile || l == Layer::AlienBomb) Kill();
		}
	};
}
