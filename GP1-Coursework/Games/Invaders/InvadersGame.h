#pragma once
#include "../IGame.h"
#include "../../Engine/GameContext.h"
#include "../../Engine/GameObject.h"
#include <memory>
#include <vector>

class Player;

class InvadersGame : public IGame {
public: 
	explicit InvadersGame(GameContext& ctx);

	void Update(float deltaTime) override;
	void Draw(Console& console) const override;
	bool IsFinished() const override { return _isFinished; }

private:
	void SpawnAliens();
	void SpawnBarriers();

	Input* _input;
	Player* _player = nullptr;
	std::vector<std::unique_ptr<GameObject>> _objects;
	bool _isFinished = false;
};