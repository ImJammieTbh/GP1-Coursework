#include "sPlayer.h"
#include "../Config.h"
#include <algorithm>

#include <memory>
#include "SnakeGame.h"

namespace Snake {

	sPlayer::sPlayer(Input& input, SnakeGame& game, float x, float y)
		: GameObject(x, y, 1, 1, 9608, 6), _input(&input), _game(&game) {}

	void sPlayer::Update(float deltaTime) {
		if (_input->IsKeyHeld(VK_LEFT) && _dirX != 1) { _nextXdir = -1; _nextYdir = 0; }
		if (_input->IsKeyHeld(VK_RIGHT) && _dirX != -1) { _nextXdir = 1; _nextYdir = 0; }
		if (_input->IsKeyHeld(VK_UP) && _dirY != 1) { _nextXdir = 0; _nextYdir = -1; }
		if (_input->IsKeyHeld(VK_DOWN) && _dirY != -1) { _nextXdir = 0; _nextYdir = 1; }

		_moveTimer += deltaTime;
		if (_moveTimer < moveInterval) return;
		_moveTimer -= moveInterval;

		_dirX = _nextXdir;
		_dirY = _nextYdir;
		m_x += static_cast<float>(_dirX);
		m_y += static_cast<float>(_dirY);

		//clamp to the walls so the player stays inside the play area
		m_x = std::clamp(m_x,
			static_cast<float>(SpaceInvaders::PlayMinX),
			static_cast<float>(SpaceInvaders::PlayMaxX - m_width));
	}
}
