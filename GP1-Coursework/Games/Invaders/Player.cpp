#include "Player.h"
#include "../Config.h"
#include <algorithm>

#include "InvadersGame.h"

namespace SpaceInvaders {

	Player::Player(Input& input, InvadersGame& game, float x, float y)
		: GameObject(x, y, 1, 1, L'^', 6), _input(&input), _game(&game) {}

	void Player::Update(float deltaTime) {
		if (_input->IsKeyHeld(VK_LEFT))
			m_x -= speed * deltaTime;
		if (_input->IsKeyHeld(VK_RIGHT))
			m_x += speed * deltaTime;
		//clamp to the walls so the player stays inside the play area
		m_x = std::clamp(m_x,
			static_cast<float>(PlayMinX),
			static_cast<float>(PlayMaxX - m_width));

		if (_input->IsKeyPressed(VK_SPACE)) {
			_game->SpawnMissile(X(), Y() - 1); //spawn missile above the player
		}
	}

}
