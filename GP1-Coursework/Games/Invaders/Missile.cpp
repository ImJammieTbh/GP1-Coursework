#include "Missile.h"
#include "../Config.h"

namespace SpaceInvaders {

	void Missile::Update(float deltaTime) {
		_timer += deltaTime;
		if (_timer >= moveInterval) {
			_timer -= moveInterval;
			MoveUp();
			if (Y() < BoundsTopY) Kill();
		}
	}

}
