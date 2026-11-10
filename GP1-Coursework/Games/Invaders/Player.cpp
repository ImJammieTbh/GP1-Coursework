#include "Player.h"
#include "Config.h"
#include <algorithm>

Player::Player(Input& input, float x, float y)
	: GameObject(x, y, 1, 1, L'^', 6), m_input(&input) {}

void Player::Update(float deltaTime) {
	if (m_input->IsKeyHeld('A')) 
		m_x -= speed * deltaTime;
	if (m_input->IsKeyHeld('D')) 
		m_x += speed * deltaTime;
	// Clamp the player position to the screen bounds
	m_x = std::clamp(m_x, 0.0f, static_cast<float>(Invaders::screenWidth - m_width));
}