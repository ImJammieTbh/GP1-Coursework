#pragma once

#include "Collision.h"
#include "Console.h"

class GameObject {
public:
	GameObject(float x, float y, float width, float height, wchar_t ch, WORD colour);
	virtual ~GameObject() = default;

	GameObject(const GameObject&) = delete;
	GameObject& operator=(const GameObject&) = delete;

	virtual void Update(float deltaTime); //default does nothing
	virtual void Draw(Console& console) const; //by default it fills its box with icon and colour

	Rect Bounds() const { return Rect{m_x, m_y, m_x + static_cast<float>(m_width), m_y + static_cast<float>(m_height)}; }

	float X() const { return m_x; }
	float Y() const { return m_y; }
	void SetPosition(float x, float y) { m_x = x; m_y = y; }

	bool IsAlive() const { return m_isAlive; }
	void Kill() { m_isAlive = false; } //flag

protected:
	float m_x, m_y;
	float m_width, m_height;
	wchar_t m_icon;
	WORD m_colour;
	bool m_isAlive = true;
};