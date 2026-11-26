#pragma once
#include "../../Engine/GameObject.h"
#include "../Config.h"

namespace SpaceInvaders {
	class Ground : public GameObject {
	public:
		Ground() : GameObject(0.0f, static_cast<float>(GroundY), screenWidth, 1, L'_', 7) {}
	};
}
