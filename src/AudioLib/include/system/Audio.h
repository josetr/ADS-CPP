#pragma once

#include "AudioDevice.h"
#include <vector>

namespace System::Audio
{
    using Devices = std::vector<AudioDevice>;
    void SetDefaultDevice(const std::wstring &deviceId);
    std::unique_ptr<AudioDevice> GetDefaultDevice();
    Devices GetDevices();
};
