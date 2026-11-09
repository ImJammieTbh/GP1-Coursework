#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include <string>
#include <vector>

class Console {
public:
    Console(short width, short height);
    ~Console();

    Console(const Console&) = delete;
    Console& operator=(const Console&) = delete;

    void Clear();
    void Draw(short x, short y, wchar_t ch, WORD colour = 7);
    void DrawString(short x, short y, const std::wstring& text, WORD colour = 7);
    void Present();

    short Width() const { return m_w; }
    short Height() const { return m_h; }

private:
    HANDLE m_out;
    short m_w, m_h;
    std::vector<CHAR_INFO> m_buffer;
};
