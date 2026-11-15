#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

#include <string>
#include <vector>

class Console {
public:
    Console(short width, short height, const wchar_t* title = L"Game");
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
    void ResizeHostWindow();

    HANDLE m_out;
    short m_w, m_h;
    std::vector<CHAR_INFO> m_buffer;

	static constexpr int kCharPixelWidth = 10;
	static constexpr int kCharPixelHeight = 22;
};
