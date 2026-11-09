#include "Console.h"

Console::Console(short width, short height)
    : m_out(GetStdHandle(STD_OUTPUT_HANDLE)),
    m_w(width), m_h(height),
    m_buffer(static_cast<size_t>(width)* height)
{
    // Order matters: shrink the window first, then set the buffer, then grow the window.
    SMALL_RECT tiny{ 0, 0, 1, 1 };
    SetConsoleWindowInfo(m_out, TRUE, &tiny);

    SetConsoleScreenBufferSize(m_out, COORD{ m_w, m_h });

    SMALL_RECT window{ 0, 0, static_cast<SHORT>(m_w - 1), static_cast<SHORT>(m_h - 1) };
    SetConsoleWindowInfo(m_out, TRUE, &window);

    CONSOLE_CURSOR_INFO cursor{ 1, FALSE };      // hide the blinking cursor
    SetConsoleCursorInfo(m_out, &cursor);

    Clear();
}

Console::~Console() {
    CONSOLE_CURSOR_INFO cursor{ 1, TRUE };       // put the cursor back
    SetConsoleCursorInfo(m_out, &cursor);
}

void Console::Clear() {
    for (CHAR_INFO& c : m_buffer) {
        c.Char.UnicodeChar = L' ';
        c.Attributes = 7;
    }
}

void Console::Draw(short x, short y, wchar_t ch, WORD colour) {
    if (x < 0 || y < 0 || x >= m_w || y >= m_h) return;   // clip, never write out of bounds
    CHAR_INFO& c = m_buffer[static_cast<size_t>(y) * m_w + x];
    c.Char.UnicodeChar = ch;
    c.Attributes = colour;
}

void Console::DrawString(short x, short y, const std::wstring& text, WORD colour) {
    for (size_t i = 0; i < text.size(); ++i)
        Draw(static_cast<short>(x + i), y, text[i], colour);
}

void Console::Present() {
    SMALL_RECT region{ 0, 0, static_cast<SHORT>(m_w - 1), static_cast<SHORT>(m_h - 1) };
    WriteConsoleOutputW(m_out, m_buffer.data(), COORD{ m_w, m_h }, COORD{ 0, 0 }, &region);
}