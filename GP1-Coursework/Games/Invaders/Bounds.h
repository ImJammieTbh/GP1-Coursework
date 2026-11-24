#pragma once
#include "../../Engine/GameObject.h"

class Bounds : public GameObject {
public:
	Bounds(float x, float y) : GameObject(x, y, 1, 1, L'|', 8) {}
};