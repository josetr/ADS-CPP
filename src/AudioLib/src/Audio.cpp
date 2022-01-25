#include "system/Audio.h"
#include <atlbase.h>
#include <initguid.h>
#include <Mmdeviceapi.h>
#include <vector>
#include <string>

#include "system/ComException.h"
#include "system/AudioDevice.h"
#include "3rdparty/PolicyConfig.h"

void System::Audio::SetDefaultDevice(const std::wstring &deviceId)
{
    CComPtr<IPolicyConfigVista> policy_config_vista_client = nullptr;
    ComThrowIfError(CoCreateInstance(__uuidof(CPolicyConfigVistaClient), nullptr, CLSCTX_ALL, __uuidof(IPolicyConfigVista), (LPVOID *)&policy_config_vista_client));
    ComThrowIfError(policy_config_vista_client->SetDefaultEndpoint(deviceId.c_str(), eConsole));
}

std::unique_ptr<AudioDevice> System::Audio::GetDefaultDevice()
{
    CComPtr<IMMDeviceEnumerator> device_enumerator = nullptr;
    ComThrowIfError(CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (LPVOID *)&device_enumerator));
    CComPtr<IMMDevice> defaultDevice = nullptr;
    const auto result = device_enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
    if (result == 0x80070490) // Element Not Found
        return {};
    ComThrowIfError(result);
    LPWSTR id = nullptr;
    ComThrowIfError(defaultDevice->GetId(&id));
    return std::make_unique<AudioDevice>(id);
}

System::Audio::Devices System::Audio::GetDevices()
{
    System::Audio::Devices devices;
    for (const auto state : {DEVICE_STATE_ACTIVE, DEVICE_STATE_DISABLED, DEVICE_STATE_UNPLUGGED, DEVICE_STATE_NOTPRESENT})
    {
        CComPtr<IMMDeviceEnumerator> device_enumerator = nullptr;
        ComThrowIfError(CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (LPVOID *)&device_enumerator));
        CComPtr<IMMDeviceCollection> device_collection = nullptr;
        ComThrowIfError(device_enumerator->EnumAudioEndpoints(eRender, state, &device_collection));
        UINT devicesCount = 0;
        ComThrowIfError(device_collection->GetCount(&devicesCount));

        for (UINT i = 0; i < devicesCount; i++)
        {
            CComPtr<IMMDevice> device;
            ComThrowIfError(device_collection->Item(i, &device));
            LPWSTR id;
            ComThrowIfError(device->GetId(&id));
            devices.emplace_back(id);
            if (devices.back().name().empty())
                devices.pop_back();
        }
    }
    return devices;
}
