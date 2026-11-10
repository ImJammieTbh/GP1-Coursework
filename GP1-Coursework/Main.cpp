#include "Engine/Console.h"
#include "Engine/Input.h"
#include "Engine/Timer.h"
#include "Engine/GameContext.h"
#include "Games/IGame.h"
#include "Games/DummyGame.h"
#include "Games/Invaders/InvadersGame.h"
#include "Menu/Menu.h"

#include <memory>

int main() {
    Console console(80, 30);
    Input input;
    Timer timer;
    GameContext context{ console, input };

    Menu menu;
    menu.Add(L"Space Invaders", [](GameContext& c) { return std::make_unique<InvadersGame>(c); });
    menu.Add(L"Game 2", [](GameContext& c) { return std::make_unique<DummyGame>(c, L"Game 2"); });

    std::unique_ptr<IGame> game;         // owns the running game, empty while in the menu

    while (!menu.WantsToQuit()) {
        const float deltaTime = timer.Tick();
        input.Update();

        if (game) {
            game->Update(deltaTime);
            if (game->IsFinished()) game.reset();    // destroys the game, back to menu
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