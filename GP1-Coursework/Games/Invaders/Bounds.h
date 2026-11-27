#pragma once
#include "../../Engine/GameObject.h"

namespace SpaceInvaders {
	class Bounds : public GameObject {
	public:
		Bounds(float x, float y) : GameObject(x, y, 1, 1, L'|', 8) {}

		Layer GetLayer() const override { return Layer::Bounds; }
	};
}
