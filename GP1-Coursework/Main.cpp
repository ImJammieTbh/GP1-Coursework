#include "Engine/Console.h"
#include "Engine/Input.h"
#include "Engine/Timer.h"
#include <string>

int main() {
	Console console(80, 25);
	Input input;
	Timer timer;

	float x = 40.f, y = 12.f;
	const float speed = 20.f; //cps cell per second

	bool running = true;
	while (running) {
		const float deltaTime = timer.Tick();
		input.Update();

		if (input.IsKeyPressed(VK_ESCAPE)) { running = false; }

		if (input.IsKeyHeld(VK_LEFT)) { x -= speed * deltaTime; }
		if (input.IsKeyHeld(VK_RIGHT)) { x += speed * deltaTime; }
		if (input.IsKeyHeld(VK_UP)) { y -= speed * deltaTime; }
		if (input.IsKeyHeld(VK_DOWN)) { y += speed * deltaTime; }

		if (x < 0.0f) { x = 0.0f; }
		if (x > console.Width() - 1) { x = static_cast<float>(console.Width() - 1); }
		if (y < 0.0f) { y = 0.0f; }
		if (y > console.Height() - 1) { y = static_cast<float>(console.Height() - 1); }

		console.Clear();
		console.DrawString(1, 0, L"Arrows to move, Escape to exit");
		console.DrawString(1, 1, L"DeltaTime: " + std::to_wstring(deltaTime));
		console.Draw(static_cast<short>(x), static_cast<short>(y), L'@', 14);
		console.Present();
	}
	return 0;
}