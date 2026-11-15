#include "Input.h"

#ifndef NOMINMAX
#define NOMINMAX
#endif // !NOMINMAX
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

#include <Windows.h>

void Input::Update() {
	m_KeysPrevious = m_KeysCurrent;
	for (int i = 0; i < 256; ++i) {
		m_KeysCurrent[i] = (GetAsyncKeyState(i) & 0x8000) != 0;
	}
}

bool Input::IsKeyHeld(int key) const {
	return key >= 0 && key < 256 && m_KeysCurrent[key];
}

bool Input::IsKeyPressed(int key) const {
	return key >= 0 && key < 256 && m_KeysCurrent[key] && !m_KeysPrevious[key];
}
