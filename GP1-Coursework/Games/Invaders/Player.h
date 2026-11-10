#pragma once
#include "../../Engine/GameObject.h"
#include "../../Engine/Input.h"

class Player : public GameObject {
public:
	Player(Input& input, float x, float y);
	void Update(float deltaTime) override;

private:
	Input* m_input;
	static constexpr float speed = 20.0f; //cps
};