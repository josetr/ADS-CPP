#pragma once

#include <optional>
#include "system/Audio.h"

namespace AudioUtil
{
    inline size_t FindDeviceWithLongestName(const System::Audio::Devices &devices)
    {
        size_t result = 0;

        for (const auto &device : devices)
        {
            if (device.name().size() > result)
                result = device.name().size();
        }

        return result;
    };

    inline std::optional<AudioDevice> FindDeviceByNameOrId(const System::Audio::Devices &devices, const std::wstring &name)
    {
        for (const auto &device : devices)
        {
            if (device.name() == name || device.id() == name)
                return device;
        }

        return std::nullopt;
    }
}
