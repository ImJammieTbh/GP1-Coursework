#pragma once

#include <chrono>

class Timer {
public:
	Timer();
	float Tick(int maxFPS = 60);

private:
	using Clock = std::chrono::steady_clock;
	Clock::time_point m_LastTime;
};