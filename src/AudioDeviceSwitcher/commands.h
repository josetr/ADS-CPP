#pragma once

#include "ut.h"
#include <optional>
#include <algorithm>

#include "system/Audio.h"
#include "system/Console.h"
#include "audio-util.h"

namespace Commands
{
    inline bool SetDefaultDevice(const std::wstring &nameOrId, std::wostream &output)
    {
        const auto device = AudioUtil::FindDeviceByNameOrId(System::Audio::GetDevices(), nameOrId);
        if (device == std::nullopt)
        {
            output << "Couldn't find any device that has a name or id equal to " << Quote(nameOrId)
                   << ".\n";
            return false;
        }

        if (device->state() == AudioDeviceState::Disabled)
        {
            output << "The device " << Quote(nameOrId) << " is disabled.\n"
                   << "Please go to the Control Panel and enable it manually.";
            return false;
        }

        System::Audio::SetDefaultDevice(device->id());
        return true;
    }

    inline bool ListDevices(std::wostream &out)
    {
        using System::Console;

        Console::SetTextColor(Console::Color::Green);
        out << "Devices:\n";

        Console::SetTextColor(Console::Color::Normal);
        const auto devices = System::Audio::GetDevices();
        const auto deviceId = System::Audio::GetDefaultDevice()->id();
        const auto namePadding = AudioUtil::FindDeviceWithLongestName(devices);
        const auto statePadding = 10;

        for (const auto &device : devices)
        {
            out << "  ";

            if (device.id() == deviceId)
            {
                Console::SetTextColor(Console::Color::Green);
                out << Pad(L"Default", statePadding);
            }
            else
            {
                Console::SetTextColor(device.state() == AudioDeviceState::Active ? Console::Color::Red : Console::Color::Normal);
                out << Pad(ToString(device.state()), statePadding);
            }

            Console::SetTextColor(Console::Color::Normal);
            out << "\t" << Pad(device.name(), namePadding) << "\n";
        }

        out << "\n";
        return true;
    }

    inline bool Toggle(std::vector<std::wstring> deviceNames, std::wostream &output)
    {
        const auto devices = System::Audio::GetDevices();
        const auto defaultDevice = System::Audio::GetDefaultDevice();

        while (!deviceNames.empty())
        {
            // Next device
            auto nextNameIt = deviceNames.begin();

            if (defaultDevice)
            {
                auto it = find(deviceNames.begin(), deviceNames.end(), defaultDevice->name());
                if (it != deviceNames.end() && std::next(it) != deviceNames.end())
                    nextNameIt = std::next(it);
            }

            if (auto dev = AudioUtil::FindDeviceByNameOrId(devices, *nextNameIt))
            {
                if (dev->state() == AudioDeviceState::Disabled)
                {
                    output << "The device " << Quote(*nextNameIt) << " is disabled.\n"
                           << "Please go to the Control Panel and enable it manually.";
                    return false;
                }
                System::Audio::SetDefaultDevice(dev->id());
                return true;
            }

            //  Keep trying until we run out of options!
            deviceNames.erase(nextNameIt);
        }

        return false;
    }
}
