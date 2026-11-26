#include "Bomb.h"

namespace SpaceInvaders {

	void Bomb::Update(float deltaTime) {
		_timer += deltaTime;
		if (_timer >= moveInterval) {
			_timer -= moveInterval;
			SetPosition(X(), Y() + 1);
			if (Y() > BoundsBottomY) Kill();
		}
	}

}
