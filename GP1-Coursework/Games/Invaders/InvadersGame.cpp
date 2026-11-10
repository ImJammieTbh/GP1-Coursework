#include "InvadersGame.h"

#include "Config.h"
#include "Alien.h"
#include "Barrier.h"
#include "Ground.h"
#include "Player.h"

#include <algorithm>

InvadersGame::InvadersGame(GameContext& ctx) : _input(&ctx.input) {
    _objects.push_back(std::make_unique<Ground>());
    SpawnBarriers();
    SpawnAliens();

    auto player = std::make_unique<Player>(ctx.input, 0.0f, static_cast<float>(Invaders::PlayerY));
    _player = player.get();                 // observe it...
    _objects.push_back(std::move(player));  // ...then hand ownership to the vector
}

void InvadersGame::SpawnAliens() {
    for (int i = 0; i < Invaders::AlienCount; ++i)
        _objects.push_back(std::make_unique<Alien>(static_cast<float>(i * 3), 1.0f));
}

void InvadersGame::SpawnBarriers() {
    for (int group = 0; group < 4; ++group)          // groups start at x = 10, 30, 50, 70
        for (int i = 0; i < 5; ++i)
            _objects.push_back(std::make_unique<Barrier>(
                static_cast<float>(10 + group * 20 + i),
                static_cast<float>(Invaders::barrierY)));
}

void InvadersGame::Update(float dt) {
    if (_input->IsKeyPressed(VK_ESCAPE)) { _isFinished = true; return; }

    for (auto& obj : _objects) obj->Update(dt);

    // Remove dead objects at the end of the frame, never mid-update.
    _objects.erase(
        std::remove_if(_objects.begin(), _objects.end(),
            [](const std::unique_ptr<GameObject>& o) { return !o->IsAlive(); }),
        _objects.end());
}

void InvadersGame::Draw(Console& console) const {
    for (const auto& obj : _objects) obj->Draw(console);
}