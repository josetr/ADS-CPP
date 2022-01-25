#pragma once

#include <stdexcept>

class WideException : public std::exception
{
public:
    char const* what() const override
    {
        return "This is a wchar_t exception, please use w_what()";
    }

    virtual wchar_t const* w_what() const
    {
        return L"Unknown wide exception";
    }
};

class BasicWideException : public WideException
{
public:
    BasicWideException(std::wstring w) : w_(w) {}

    wchar_t const* w_what() const override
    {
        return w_.c_str();
    }

private:
    std::wstring w_;
};
