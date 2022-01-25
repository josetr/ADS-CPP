#include "system/ComInstance.h"
#include "system/ComException.h"
#include <comdef.h>

ComInstance::ComInstance()
{
    ComThrowIfError(CoInitialize(nullptr));
}

ComInstance::~ComInstance()
{
    CoUninitialize();
}
