#include "system//ComException.h"
#include <comdef.h>

ComException::ComException(long result) : result_(result), message_(_com_error(result).ErrorMessage())
{
}

const wchar_t *ComException::w_what() const
{
    return message_.c_str();
}

const std::wstring &ComException::message() const
{
    return message_;
}

long ComException::errorCode() const
{
    return result_;
}

void ComThrowIfError(long result)
{
    if (!SUCCEEDED(result))
        throw ComException(result);
}
