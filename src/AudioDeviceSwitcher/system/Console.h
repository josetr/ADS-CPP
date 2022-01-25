#pragma once

#include <ostream>

namespace System
{
    class Console
    {
    public:
        enum class Color
        {
            Blue = 1 << 0,
            Green = 1 << 1,
            Red = 1 << 2,
            Normal = Blue | Green | Red
        };

        Console();
        ~Console();
        void maximize();
        void size(short rows, short columns);
        void title(const std::wstring& title);
        static void SetTextColor(Color color);
    };
}
