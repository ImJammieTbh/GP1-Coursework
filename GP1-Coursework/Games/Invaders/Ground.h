#pragma once
#include "../../Engine/GameObject.h"
#include "Config.h"

class Ground : public GameObject {
public:
	Ground() : GameObject(0.0f, static_cast<float>(Invaders::GroundY), Invaders::screenWidth, 1, L'_', 7) {}
};
