#include "GameObject.h"

GameObject::GameObject(float x, float y, float width, float height, wchar_t icon, WORD colour)
	: m_x(x), m_y(y), m_width(width), m_height(height), m_icon(icon), m_colour(colour) {}

void GameObject::Update(float deltaTime) {}

void GameObject::Draw(Console& console) const {
	for (int i = 0; i < m_height; ++i) {
		for (int col = 0; col < m_width; ++col) {
			console.Draw(
				static_cast<short>(m_x + col), 
				static_cast<short>(m_y + i),
				m_icon, 
				m_colour
			);
		}
	}
}