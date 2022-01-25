#pragma once

#include "WideException.h"
#include <iostream>
#include <cassert>

class ComException : public WideException
{
public:
    explicit ComException(long result);
    virtual const wchar_t* w_what() const override;
    const std::wstring& message() const;
    long errorCode() const;
private:
    long result_;
    std::wstring message_;
};

void ComThrowIfError(long result);