#pragma once

#include "../../Engine/GameObject.h"

namespace Snake {
	class Apple : public GameObject {
		Apple(float x, float y) : GameObject(x, y, 1, 1, L'O', Colour::Red) {}

		Layer GetLayer() const override { return Layer::Fruit; }
		void OnHit(GameObject& other) override {
			Layer l = other.GetLayer();
			if (l == Layer::Player) Kill();
		}
	};
}