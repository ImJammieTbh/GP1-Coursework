#include "Engine/Console.h"
#include "Engine/Input.h"
#include "Engine/Timer.h"
#include "Engine/GameContext.h"
#include "Games/IGame.h"
#include "Games/DummyGame.h"
#include "Games/Invaders/InvadersGame.h"
#include "Games/Snake/SnakeGame.h"
#include "Menu/Menu.h"

#include <memory>

int main() { //help me 
    Console console(80, 30, L"GP1-GameLoader");
    Input input;
    Timer timer;
    GameContext context{ console, input };

    Menu menu;
    menu.Add(L"Space Invaders", [](GameContext& c) { return std::make_unique<SpaceInvaders::InvadersGame>(c); });
    menu.Add(L"Snake", [](GameContext& c) { return std::make_unique<Snake::SnakeGame>(c); });

    std::unique_ptr<IGame> game;         //owns the running game, empty while in the menu

    while (!menu.WantsToQuit()) {
        const float deltaTime = timer.Tick();
        input.Update();

        if (game) {
            game->Update(deltaTime);
            if (game->IsFinished()) game.reset();    //destroys the game, back to menu
        }
        else {
            game = menu.Update(context);
        }

        console.Clear();
        if (game) game->Draw(console);
        else      menu.Draw(console);
        console.Present();
    }
    return 0;
}