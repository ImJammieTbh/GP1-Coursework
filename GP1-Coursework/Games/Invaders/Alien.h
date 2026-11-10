#pragma once
#include "../../Engine/GameObject.h"

class Alien : public GameObject {
public:
	Alien(float x, float y) : GameObject(x, y, 1, 1, L'A', 7) {}
};