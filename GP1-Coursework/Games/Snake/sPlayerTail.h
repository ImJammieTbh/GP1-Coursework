#pragma once
#include "../../Engine/GameObject.h"

namespace Snake {
	class sPlayerTail : public GameObject {
	public:
		sPlayerTail(float x, float y) : GameObject(x, y, 1, 1, 9608, Colour::LightBlue) {}
		
		Layer GetLayer() const override { return Layer::PlayerTail; }
	};
}