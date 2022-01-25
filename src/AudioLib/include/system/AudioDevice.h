#pragma once

#include <memory>
#include <string>
#include <stdexcept>

enum class AudioDeviceState
{
    Active = 1,
    Disabled = 2,
    NotPresent = 4,
    Unplugged = 8,
};

enum class AudioEndpointFormFactor
{
    RemoteNetworkDevice,
    Speakers,
    LineLevel,
    Headphones,
    Microphone,
    Headset,
    Handset,
    UnknownDigitalPassthrough,
    SPDIF,
    DigitalAudioDisplayDevice,
    UnknownFormFactor
};

class AudioDevice
{
public:
    AudioDevice(const std::wstring& id);
    AudioDevice(const AudioDevice& other);
    AudioDevice(AudioDevice&& other) noexcept;
    ~AudioDevice();
    AudioDevice& operator=(const AudioDevice& other);
    AudioDevice& operator=(AudioDevice&& other) noexcept;
public:
    std::wstring id() const;
    std::wstring name() const;
    AudioEndpointFormFactor formFactor() const;
    AudioDeviceState state() const;
private:
    class AudioDeviceImp;
    std::unique_ptr<AudioDeviceImp> impl_;
};

inline std::wstring ToString(AudioDeviceState state)
{
    switch (state)
    {
    case AudioDeviceState::Active: return L"Active";
    case AudioDeviceState::Disabled: return L"Disabled";
    case AudioDeviceState::NotPresent: return L"NotPresent";
    case AudioDeviceState::Unplugged: return L"Unplugged";
    default: throw std::runtime_error("invalid AudioDeviceState");
    };
}
