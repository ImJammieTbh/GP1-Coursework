#pragma once
#include "../../Engine/GameObject.h"

class Barrier : public GameObject {
public:
	Barrier(float x, float y) : GameObject(x, y, 1, 1, L'=', 7) {}
};