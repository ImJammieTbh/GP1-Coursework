#pragma once

#include "../Engine/Console.h"

class IGame {
public:
	virtual ~IGame() = default;

	virtual void Update(float deltaTime) = 0;
	virtual void Draw(Console& console) const = 0;
	virtual bool IsFinished() const = 0;// if true the go back to loader menu
};