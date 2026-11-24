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

namespace Colour {
	constexpr WORD Black = 0x0;
	constexpr WORD Blue = 0x1;
	constexpr WORD Green = 0x2;
	constexpr WORD Cyan = 0x3;
	constexpr WORD Red = 0x4;
	constexpr WORD Magenta = 0x5;
	constexpr WORD Yellow = 0x6;
	constexpr WORD LightGrey = 0x7;
	constexpr WORD DarkGrey = 0x8;
	constexpr WORD LightBlue = 0x9;
	constexpr WORD LightGreen = 0xA;
	constexpr WORD LightCyan = 0xB;
	constexpr WORD LightRed = 0xC;
	constexpr WORD LightMagenta = 0xD;
	constexpr WORD LightYellow = 0xE;
	constexpr WORD White = 0xF;

	constexpr WORD Make(WORD fg, WORD bg = Black) { return static_cast<WORD>((bg << 4) | fg); }
}
