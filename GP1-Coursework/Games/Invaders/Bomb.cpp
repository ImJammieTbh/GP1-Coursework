#include "Bomb.h"

void Bomb::Update(float deltaTime) {
	_timer += deltaTime;
	if (_timer >= moveInterval) {
		_timer -= moveInterval;
		SetPosition(X(), Y() + 1);
		if (Y() > Invaders::BoundsBottomY) Kill();
	}
}