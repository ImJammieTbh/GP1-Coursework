#pragma once

#include <array>

class Input {
public:
	//void Init();
	void Update();
	bool IsKeyPressed(int key) const;
	//bool IsKeyReleased(int key) const;
	bool IsKeyHeld(int key) const;

private:
	std::array<bool, 256> m_KeysCurrent{};
	std::array<bool, 256> m_KeysPrevious{};
};