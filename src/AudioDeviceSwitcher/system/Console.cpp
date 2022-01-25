#include "Console.h"
#include <windows.h>
#include <cstdio>
#include <iostream>

using namespace System;

Console::Console()
{
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
    freopen("CONIN$", "r", stdin);
}

Console::~Console()
{
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    FreeConsole();
}

void Console::title(const std::wstring &title)
{
    SetConsoleTitle(title.c_str());
}

void Console::maximize()
{
    const auto handle = GetStdHandle(STD_OUTPUT_HANDLE);
    const auto coords = GetLargestConsoleWindowSize(handle);
    const auto rect = SMALL_RECT{0, 0, static_cast<short>(coords.X - 1), static_cast<short>(coords.Y - 1)};
    SetConsoleScreenBufferSize(handle, coords);
    SetConsoleWindowInfo(handle, true, &rect);
    ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
}

void Console::size(short rows, short columns)
{
    const auto handle = GetStdHandle(STD_OUTPUT_HANDLE);
    auto coords = GetLargestConsoleWindowSize(handle);
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(handle, &info);

    if (columns > info.srWindow.Right &&
        rows > info.srWindow.Bottom &&
        columns <= coords.X &&
        rows <= coords.Y)
    {
        coords.X = columns;
        coords.Y = rows;
        const SMALL_RECT rect{0, 0, coords.X - 1, coords.Y - 1};
        SetConsoleScreenBufferSize(handle, coords);
        SetConsoleWindowInfo(handle, true, &rect);
    }
}

void Console::SetTextColor(Console::Color color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), static_cast<int>(color));
}
