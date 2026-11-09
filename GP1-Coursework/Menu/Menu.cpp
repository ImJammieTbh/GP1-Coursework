#include "Menu.h"

void Menu::Add(std::wstring name, Factory factory) {
    m_entries.push_back(Entry{ std::move(name), std::move(factory) });
}

std::unique_ptr<IGame> Menu::Update(GameContext& context) {
    const int count = static_cast<int>(m_entries.size());
    if (count == 0) return nullptr;

    if (context.input.IsKeyPressed(VK_UP))   m_selectedIndex = (m_selectedIndex + count - 1) % count;
    if (context.input.IsKeyPressed(VK_DOWN)) m_selectedIndex = (m_selectedIndex + 1) % count;

    if (context.input.IsKeyPressed(VK_ESCAPE)) m_wantsToQuit = true;

    if (context.input.IsKeyPressed(VK_RETURN))
        return m_entries[m_selectedIndex].createFactory(context);

    return nullptr;
}

void Menu::Draw(Console& console) const {
    console.DrawString(32, 5, L"GAME SELECT", 14);

    for (int i = 0; i < static_cast<int>(m_entries.size()); ++i) {
        const bool selected = (i == m_selectedIndex);
        const std::wstring line = (selected ? L"> " : L"  ") + m_entries[i].name;
        console.DrawString(30, static_cast<short>(8 + i * 2), line, selected ? 15 : 7);
    }

    console.DrawString(18, 22, L"Up/Down to choose, Enter to play, Esc to quit", 8);
}