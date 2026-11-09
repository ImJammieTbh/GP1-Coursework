#include "Engine/Console.h"

int main() {
    Console console(80, 25);

    for (int x = 0; x < 78; ++x) {
        console.Clear();
        console.DrawString(1, 1, L"Console test");
        console.Draw(static_cast<short>(x), 10, L'@', 14);   // 14 = yellow
        console.Present();
        Sleep(33);                                          // ~30 FPS, temporary
    }
    return 0;
}