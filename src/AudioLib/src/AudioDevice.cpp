#include "system/AudioDevice.h"
#include "system/ComException.h"
#include <atlbase.h>
#include <initguid.h>
#include <Mmdeviceapi.h>
#include <Functiondiscoverykeys_devpkey.h>

class AudioDevice::AudioDeviceImp
{
public:
    AudioDeviceImp(CComPtr<IMMDevice> device) : device_(device)
    {
    }

    AudioDeviceImp(const std::wstring &id)
    {
        CComPtr<IMMDeviceEnumerator> fuckinEnumerator = nullptr;
        ComThrowIfError(CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (LPVOID *)&fuckinEnumerator));
        ComThrowIfError(fuckinEnumerator->GetDevice(id.c_str(), &device_));
    }

    CComPtr<IPropertyStore> properties() const
    {
        CComPtr<IPropertyStore> properties;
        ComThrowIfError(device_->OpenPropertyStore(STGM_READ, &properties));
        return properties;
    }

    std::wstring id() const
    {
        LPWSTR id = nullptr;
        ComThrowIfError(device_->GetId(&id));
        return id;
    }

    AudioEndpointFormFactor formFactor() const
    {
        PROPVARIANT variant;
        PropVariantInit(&variant);
        ComThrowIfError(properties()->GetValue(PKEY_AudioEndpoint_FormFactor, &variant));
        const auto form_factor = variant.uintVal;
        PropVariantClear(&variant);

        switch (form_factor)
        {
        case RemoteNetworkDevice:
            return AudioEndpointFormFactor::RemoteNetworkDevice;
        case Speakers:
            return AudioEndpointFormFactor::Speakers;
        case LineLevel:
            return AudioEndpointFormFactor::LineLevel;
        case Headphones:
            return AudioEndpointFormFactor::Headphones;
        case Microphone:
            return AudioEndpointFormFactor::Microphone;
        case Headset:
            return AudioEndpointFormFactor::Headset;
        case Handset:
            return AudioEndpointFormFactor::Handset;
        case UnknownDigitalPassthrough:
            return AudioEndpointFormFactor::UnknownDigitalPassthrough;
        case SPDIF:
            return AudioEndpointFormFactor::SPDIF;
        case DigitalAudioDisplayDevice:
            return AudioEndpointFormFactor::DigitalAudioDisplayDevice;
        case UnknownFormFactor:
            return AudioEndpointFormFactor::UnknownFormFactor;
        default:
            throw std::runtime_error("Invalid EndpointFormFactor");
        }
    }

    std::wstring name() const
    {
        std::wstring name;
        PROPVARIANT variant;
        PropVariantInit(&variant);
        if (SUCCEEDED(properties()->GetValue(PKEY_Device_FriendlyName, &variant)))
        {
            if (variant.pwszVal != nullptr)
                name = variant.pwszVal;
            PropVariantClear(&variant);
        }
        return name;
    }

    AudioDeviceState state() const
    {
        DWORD state = 0;
        ComThrowIfError(device_->GetState(&state));
        switch (state)
        {
        case DEVICE_STATE_ACTIVE:
            return AudioDeviceState::Active;
        case DEVICE_STATE_DISABLED:
            return AudioDeviceState::Disabled;
        case DEVICE_STATE_NOTPRESENT:
            return AudioDeviceState::NotPresent;
        case DEVICE_STATE_UNPLUGGED:
            return AudioDeviceState::Unplugged;
        default:
            throw std::runtime_error("Invalid DeviceState");
        }
    }

private:
    CComPtr<IMMDevice> device_;
};

AudioDevice::AudioDevice(const std::wstring &id) : impl_(std::make_unique<AudioDeviceImp>(id)) {}

AudioDevice::AudioDevice(const AudioDevice &other) : impl_(std::make_unique<AudioDeviceImp>(*other.impl_)) {}

AudioDevice::AudioDevice(AudioDevice &&other) noexcept : impl_(std::move(other.impl_)) {}

AudioDevice &AudioDevice::operator=(const AudioDevice &other)
{
    impl_ = std::make_unique<AudioDeviceImp>(*other.impl_);
    return *this;
}

AudioDevice &AudioDevice::operator=(AudioDevice &&other) noexcept
{
    impl_ = std::move(other.impl_);
    return *this;
}

AudioDevice::~AudioDevice() = default;

std::wstring AudioDevice::id() const { return impl_->id(); }

std::wstring AudioDevice::name() const { return impl_->name(); }

AudioEndpointFormFactor AudioDevice::formFactor() const { return impl_->formFactor(); }

AudioDeviceState AudioDevice::state() const { return impl_->state(); }
