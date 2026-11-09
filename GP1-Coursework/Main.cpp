#include "Engine/Console.h"
#include "Engine/Input.h"
#include "Engine/Timer.h"
#include "Engine/GameContext.h"
#include "Games/IGame.h"
#include "Games/DummyGame.h"
#include "Menu/Menu.h"

#include <memory>

int main() {
    Console console(80, 25);
    Input input;
    Timer timer;
    GameContext ctx{ console, input };

    Menu menu;
    menu.Add(L"Game 1", [](GameContext& c) { return std::make_unique<DummyGame>(c, L"Game 1"); });
    menu.Add(L"Game 2", [](GameContext& c) { return std::make_unique<DummyGame>(c, L"Game 2"); });

    std::unique_ptr<IGame> game;         // owns the running game, empty while in the menu

    while (!menu.WantsToQuit()) {
        const float dt = timer.Tick();
        input.Update();

        if (game) {
            game->Update(dt);
            if (game->IsFinished()) game.reset();    // destroys the game, back to menu
        }
        else {
            game = menu.Update(ctx);
        }

        console.Clear();
        if (game) game->Draw(console);
        else      menu.Draw(console);
        console.Present();
    }
    return 0;
}