#include "InvadersGame.h"

#include "Config.h"
#include "Bounds.h"
#include "Alien.h"
#include "Barrier.h"
#include "Ground.h"
#include "Player.h"
#include "Bomb.h"

#include <algorithm>
#include "MIssile.h"

InvadersGame::InvadersGame(GameContext& ctx) : _input(&ctx.input) {
    _objects.push_back(std::make_unique<Ground>());
    SpawnBounds();
    SpawnBarriers();
    SpawnAliens();

    auto player = std::make_unique<Player>(ctx.input, *this, Invaders::PlayMinX, static_cast<float>(Invaders::PlayerY));
    _player = player.get();                 // observe it...
    _objects.push_back(std::move(player));  // ...then hand ownership to the vector
}

void InvadersGame::SpawnBounds() {
    for (int y = Invaders::BoundsTopY; y <= Invaders::BoundsBottomY; ++y) {
        for (int t = 0; t < Invaders::BoundsThickness; ++t) {
            _objects.push_back(std::make_unique<Bounds>(
                static_cast<float>(Invaders::BoundsLeftX + t), static_cast<float>(y)));
            _objects.push_back(std::make_unique<Bounds>(
                static_cast<float>(Invaders::BoundsRightX + t), static_cast<float>(y)));
        }
    }
}

void InvadersGame::SpawnAliens() {
	for (int row = 0; row < Invaders::alienRows; ++row) {
		for (int col = 0; col < Invaders::alienCols; ++col) {
			auto alien = std::make_unique<Alien>(
				static_cast<float>(Invaders::alienStartX + col * Invaders::alienSpacingX),
				static_cast<float>(Invaders::alienStartY + row * Invaders::alienSpacingY));
			_aliens.push_back(alien.get());          //observe it
			_objects.push_back(std::move(alien));    //then hand ownership to the vector
		}
	}
}

void InvadersGame::SpawnBarriers() {
    for (int group = 0; group < Invaders::barrierGroupCount; ++group) {
        const int groupX = Invaders::barrierX + group * (Invaders::barrierWidth + Invaders::barrierGaps);
        for (int row = 0; row < Invaders::barrierHeight; ++row) {
            for (int col = 0; col < Invaders::barrierWidth; ++col) {
                _objects.push_back(std::make_unique<Barrier>(
                    static_cast<float>(groupX + col),
                    static_cast<float>(Invaders::barrierY + row)));
            }
        }
    }
}

void InvadersGame::Update(float deltaTime) {
    if (_input->IsKeyPressed(VK_ESCAPE)) { _isFinished = true; return; }

    for (auto& obj : _objects) obj->Update(deltaTime);

    _bombTimer += deltaTime;
    if (!_gameOver && _bombTimer >= _bombDelay) {
        _bombTimer -= _bombDelay;
        AlienShoot();
    }

	for (auto& obj : _pending) _objects.push_back(std::move(obj));
	_pending.clear();

    if (!_gameOver) {
        _moveTimer += deltaTime;
        if (_moveTimer >= _moveDelay) {
            _moveTimer -= _moveDelay;
            StepAliens();
            if (AliensAtBottom()) {
                _gameOver = true;
                //_isFinished = true;
            }
        }
    }

    for (size_t i = 0; i < _objects.size(); ++i) {
        for (size_t j = i + 1; j < _objects.size(); ++j) {
            GameObject& a = *_objects[i];
            GameObject& b = *_objects[j];
            if (!a.IsAlive() || !b.IsAlive()) continue;
            if (Overlaps(a.Bounds(), b.Bounds())) {
                a.OnHit(b);
                b.OnHit(a);
            }
        }
    }

    if (_player && !_player->IsAlive()) { _player = nullptr; _gameOver = true; }
    if (_missile && !_missile->IsAlive()) _missile = nullptr;
	
	//Remove dead aliens and objects
    //Pruning pointers before the object is gone
    _aliens.erase(
		std::remove_if(_aliens.begin(), _aliens.end(),
			[](const Alien* a) { return !a->IsAlive(); }),
		_aliens.end());

    _objects.erase(
        std::remove_if(_objects.begin(), _objects.end(),
            [](const std::unique_ptr<GameObject>& o) { return !o->IsAlive(); }),
        _objects.end());

    const float t = static_cast<float>(_aliens.size()) / (Invaders::alienRows * Invaders::alienCols);
    _moveDelay = _minMoveDelay + t * (1.0f - _minMoveDelay);   // 1.0f = starting delay
}

void InvadersGame::Draw(Console& console) const {
    for (const auto& obj : _objects) obj->Draw(console);

    if (_gameOver) {
        const wchar_t* msg = L"GAME OVER - ESC to return";
        for (int i = 0; msg[i] != L'\0'; ++i)
            console.Draw(static_cast<short>(27 + i), 12, msg[i], 12);
    }
}

void InvadersGame::StepAliens() {
    bool hitEdge = false;
    for (const Alien* a : _aliens) {
        if (!a->IsAlive()) continue;
        const float nextX = a->X() + _moveDirection;
        if (nextX < Invaders::PlayMinX || nextX + 1.0f > Invaders::PlayMaxX) { hitEdge = true; break; }
    }

    for (Alien* a : _aliens) {
        if (!a->IsAlive()) continue;
        if (hitEdge) a->Move(0.0f, 1.0f);
        else         a->Move(_moveDirection, 0.0f);
    }
    if (hitEdge) {
        _moveDirection = -_moveDirection;
        _moveDelay = std::max(_moveDelay * 0.9f, _minMoveDelay);   // 10% faster each bounce
    }
}

bool InvadersGame::AliensAtBottom() const {
    for (const Alien* a : _aliens) {
        if (a->IsAlive() && a->Y() + 1.0f >= Invaders::PlayerY) return true;
    }
    return false;
}

void InvadersGame::SpawnMissile(float x, float y) {
    if (_missile) return;
    auto missile = std::make_unique<Missile>(x, y);
    _missile = missile.get();
    _pending.push_back(std::move(missile));
}

void InvadersGame::AlienShoot() {
    if (_aliens.empty()) return;
    std::uniform_int_distribution<size_t> pick(0, _aliens.size() - 1);
    const Alien* a = _aliens[pick(_rng)];
    if (!a->IsAlive()) return;
    _pending.push_back(std::make_unique<Bomb>(a->X(), a->Y() + 1));
}