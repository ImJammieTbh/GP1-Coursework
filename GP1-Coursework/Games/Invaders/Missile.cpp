#include "Missile.h"
#include "Config.h"

void Missile::Update(float deltaTime) {
	_timer += deltaTime;
	if (_timer >= moveInterval) {
		_timer -= moveInterval;
		MoveUp();
		if (Y() < Invaders::BoundsTopY) Kill();   // whatever marks it dead
	}
}