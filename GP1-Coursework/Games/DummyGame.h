#pragma once

#include "IGame.h"
#include "../Engine/GameContext.h"
#include <string>

class DummyGame : public IGame {
public:
	DummyGame(GameContext& context, const std::wstring name)
		: m_input(&context.input), m_name(name) {
	}

	void Update(float /*deltaTime*/) override {
		if (m_input->IsKeyPressed(VK_ESCAPE)) {
			m_finished = true;
		}
	}

	void Draw(Console& console) const override {
		console.DrawString(30, 10, m_name);
		console.DrawString(26, 12, L"Press ESC to exit to menu");
	}

	bool IsFinished() const override {
		return m_finished;
	}
private:
	Input* m_input; //this does not belong to you :)
	std::wstring m_name;
	bool m_finished = false;
};