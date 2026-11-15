#include "Timer.h"
#include <thread>

Timer::Timer() : m_LastTime(Clock::now()) {}

float Timer::Tick(int maxFPS) {
	const std::chrono::duration<float> frameTime(1.0f / static_cast<float>(maxFPS));

	std::chrono::duration<float> elapsed = Clock::now() - m_LastTime;
	if (elapsed < frameTime) {
		std::this_thread::sleep_for(frameTime - elapsed);
	}

	const Clock::time_point currentTime = Clock::now();
	float deltaTime = std::chrono::duration<float>(currentTime - m_LastTime).count();
	m_LastTime = currentTime;

	if (deltaTime > 0.1f) deltaTime = 0.1f;
	return deltaTime;
}