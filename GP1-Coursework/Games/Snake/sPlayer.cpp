#include "sPlayer.h"
#include "../Config.h"
#include <algorithm>

#include <memory>
#include "SnakeGame.h"

namespace Snake {

	sPlayer::sPlayer(Input& input, SnakeGame& game, float x, float y)
		: GameObject(x, y, 1, 1, 9608, 6), _input(&input), _game(&game) {}

	void sPlayer::Update(float deltaTime) {
		if (_game->isGameOver() || _game->isPlayerWon()) return;
		
		if (_input->IsKeyHeld(VK_LEFT) && _dirX != 1) { _nextXdir = -1; _nextYdir = 0; }
		if (_input->IsKeyHeld(VK_RIGHT) && _dirX != -1) { _nextXdir = 1; _nextYdir = 0; }
		if (_input->IsKeyHeld(VK_UP) && _dirY != 1) { _nextXdir = 0; _nextYdir = -1; }
		if (_input->IsKeyHeld(VK_DOWN) && _dirY != -1) { _nextXdir = 0; _nextYdir = 1; }

		_moveTimer += deltaTime;
		if (_moveTimer < moveInterval) return;
		_moveTimer -= moveInterval;

		_dirX = _nextXdir;
		_dirY = _nextYdir;

		float oldx = m_x;
		float oldY = m_y;
		
		float nextX = m_x + static_cast<float>(_dirX);
		float nextY = m_y + static_cast<float>(_dirY);
		if (_game->isWallAhead(nextX, nextY)) {
			_game->GameOver();
			return;
		}
		
		m_x = nextX;
		m_y = nextY;

		_game->SpawnTail(oldx, oldY);

		//clamp to the walls so the player stays inside the play area
		m_x = std::clamp(m_x,
			static_cast<float>(Snake::PlayMinX),
			static_cast<float>(Snake::PlayMaxX - m_width));

		m_y = std::clamp(m_y,
			static_cast<float>(Snake::BoundsTopY + m_height),
			static_cast<float>(Snake::GroundY - m_height));
	}

	void sPlayer::OnHit(GameObject& other) {
		if (other.GetLayer() == Layer::Fruit) {
			_game->_maxTailLength++;
			other.Kill(); // remove fruit after ate
		}
			
		if (other.GetLayer() == Layer::PlayerTail) {
			_game->GameOver();
		}
	}
}
