﻿
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include "ToastNotificationUtility.h"
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.Foundation.h>
#include <externs.h>
#include <frameworkudk/pushnotifications.h>

namespace winrt
{
    using namespace winrt::Windows::Foundation;
    using namespace Windows::ApplicationModel::Core;
}

std::wstring RetrieveUnpackagedToastAppId()
{
    wil::unique_cotaskmem_string processName;
    THROW_IF_FAILED(GetCurrentProcessPath(processName));

    std::wstring wideStringProcessName{ processName.get() };
    // subKey: L"Software\\Classes\\AppUserModelId\\{Path to ToastNotificationsTestApp.exe}"
    std::wstring subKey{ c_appIdentifierPath + ConvertPathToKey(wideStringProcessName) };

    wil::unique_hkey hKey;
    THROW_IF_FAILED(RegCreateKeyEx(
        HKEY_CURRENT_USER,
        subKey.c_str(),
        0,
        nullptr /* lpClass */,
        REG_OPTION_NON_VOLATILE,
        KEY_ALL_ACCESS,
        nullptr /* lpSecurityAttributes */,
        &hKey,
        nullptr /* lpdwDisposition */));

    WCHAR registeredGuidBuffer[GUID_LENGTH];
    DWORD bufferLength = sizeof(registeredGuidBuffer);
    HRESULT status = RegGetValueW(
        hKey.get(),
        nullptr /* lpValue */,
        L"ToastGUID",
        RRF_RT_REG_SZ,
        nullptr /* pdwType */,
        &registeredGuidBuffer,
        &bufferLength);

    if (status == ERROR_FILE_NOT_FOUND)
    {
        GUID newToastGuid;
        THROW_IF_FAILED(CoCreateGuid(&newToastGuid));

        wil::unique_cotaskmem_string newToastGuidString;
        THROW_IF_FAILED(StringFromCLSID(newToastGuid, &newToastGuidString));

        std::wstring guidWideStr{ newToastGuidString.get() };
        RegisterValue(hKey, L"ToastGUID", reinterpret_cast<const BYTE*>(guidWideStr.c_str()), REG_SZ, guidWideStr.size() * sizeof(wchar_t));
        return guidWideStr;
    }

    THROW_HR_IF(status, FAILED_WIN32(status));
    return registeredGuidBuffer;
}

std::wstring RetrieveToastAppId()
{
    if (AppModel::Identity::IsPackagedProcess())
    {
        wchar_t appUserModelId[APPLICATION_USER_MODEL_ID_MAX_LENGTH] = {};
        UINT32 appUserModelIdSize{ APPLICATION_USER_MODEL_ID_MAX_LENGTH };

        THROW_IF_FAILED(GetCurrentApplicationUserModelId(&appUserModelIdSize, appUserModelId));
        return appUserModelId;
    }

    return RetrieveUnpackagedToastAppId();
}

void RegisterAssets(std::wstring const& appId, winrt::Microsoft::Windows::ToastNotifications::ToastAssets const& activationInfo, wil::unique_cotaskmem_string const& clsid)
{
    wil::unique_hkey hKey;
    // subKey: \Software\Classes\AppUserModelId\{AppGUID}
    std::wstring subKey{ c_appIdentifierPath + appId };

    THROW_IF_FAILED(RegCreateKeyEx(
        HKEY_CURRENT_USER,
        subKey.c_str(),
        0,
        nullptr /* lpClass */,
        REG_OPTION_NON_VOLATILE,
        KEY_ALL_ACCESS,
        nullptr /* lpSecurityAttributes */,
        &hKey,
        nullptr /* lpdwDisposition */));

    RegisterValue(hKey, L"DisplayName", reinterpret_cast<const BYTE*>(activationInfo.DisplayName().c_str()), REG_EXPAND_SZ, activationInfo.DisplayName().size() * sizeof(wchar_t));
    RegisterValue(hKey, L"IconUri", reinterpret_cast<const BYTE*>(activationInfo.IconPath().AbsoluteUri().c_str()), REG_EXPAND_SZ, activationInfo.IconPath().AbsoluteUri().size() * sizeof(wchar_t));

    std::wstring wideStringClsid{ clsid.get() };
    RegisterValue(hKey, L"CustomActivator", reinterpret_cast<const BYTE*>(wideStringClsid.c_str()), REG_SZ, wideStringClsid.size() * sizeof(wchar_t));
}

void RegisterComServer(wil::unique_cotaskmem_string const& processName, wil::unique_cotaskmem_string const& clsid)
{
    wil::unique_hkey hKey;
    //subKey: Software\Classes\CLSID\{comActivatorGuidString}\LocalServer32
    std::wstring subKey{ c_clsIdPath + clsid.get() + LR"(\LocalServer32)" };

    THROW_IF_FAILED(RegCreateKeyEx(
        HKEY_CURRENT_USER,
        subKey.c_str(),
        0,
        nullptr /* lpClass */,
        REG_OPTION_NON_VOLATILE,
        KEY_ALL_ACCESS,
        nullptr /* lpSecurityAttributes */,
        &hKey,
        nullptr /* lpdwDisposition */));

    std::wstring comRegistrationExeString{ c_quote + processName.get() + c_quote + c_toastActivatedArgument };

    RegisterValue(hKey, nullptr, reinterpret_cast<const BYTE*>(comRegistrationExeString.c_str()), REG_SZ, (comRegistrationExeString.size() * sizeof(wchar_t)));
}

void UnRegisterComServer(std::wstring const& clsid)
{
    wil::unique_hkey hKey;
    //subKey: Software\Classes\CLSID\{comActivatorGuidString}\LocalServer32
    std::wstring subKey{ c_clsIdPath + clsid + LR"(\LocalServer32)" };

    THROW_IF_FAILED(RegDeleteKeyEx(
        HKEY_CURRENT_USER,
        subKey.c_str(),
        KEY_WOW64_64KEY,
        0));
}

void UnRegisterToastAppIdentifierFromRegistry()
{
    std::wstring toastAppId{ RetrieveToastAppId() };

    wil::unique_hkey hKey;
    //subKey: \Software\Classes\AppUserModelId\{AppGUID}
    std::wstring subKey{ c_appIdentifierPath + toastAppId };

    THROW_IF_FAILED(RegDeleteKeyEx(
        HKEY_CURRENT_USER,
        subKey.c_str(),
        KEY_WOW64_64KEY,
        0));
}

HRESULT GetActivatorGuid(std::wstring& activatorGuid) noexcept try
{
    std::wstring toastAppId{ RetrieveToastAppId() };
    // subKey: \Software\Classes\AppUserModelId\{AppGUID}
    std::wstring subKey{ c_appIdentifierPath + toastAppId };

    wil::unique_hkey hKey;
    THROW_IF_FAILED(RegCreateKeyEx(
        HKEY_CURRENT_USER,
        subKey.c_str(),
        0,
        nullptr /* lpClass */,
        REG_OPTION_NON_VOLATILE,
        KEY_ALL_ACCESS,
        nullptr /* lpSecurityAttributes */,
        &hKey,
        nullptr /* lpdwDisposition */));

    WCHAR activatorGuidBuffer[GUID_LENGTH];
    DWORD bufferLength = sizeof(activatorGuidBuffer);
    HRESULT status = RegGetValueW(
        hKey.get(),
        nullptr /* lpValue */,
        L"CustomActivator",
        RRF_RT_REG_SZ,
        nullptr /* pdwType */,
        &activatorGuidBuffer,
        &bufferLength);

    activatorGuid = activatorGuidBuffer;
    return status;
}
CATCH_RETURN()

std::wstring RegisterComActivatorGuidAndAssets(winrt::Microsoft::Windows::ToastNotifications::ToastActivationInfo const& details)
{
    std::wstring registeredGuid;
    HRESULT status = GetActivatorGuid(registeredGuid);
    if (status == ERROR_FILE_NOT_FOUND)
    {
        wil::unique_cotaskmem_string processName;
        THROW_IF_FAILED(GetCurrentProcessPath(processName));

        GUID comActivatorGuid = GUID_NULL;
        THROW_IF_FAILED(CoCreateGuid(&comActivatorGuid));

        // StringFromCLSID returns GUID String with braces.
        wil::unique_cotaskmem_string comActivatorGuidString;
        THROW_IF_FAILED(StringFromCLSID(comActivatorGuid, &comActivatorGuidString));

        std::wstring toastAppId{ RetrieveToastAppId() };
        RegisterAssets(toastAppId, details.Assets(), comActivatorGuidString);
        RegisterComServer(processName, comActivatorGuidString);

        return comActivatorGuidString.get();
    }

    THROW_IF_WIN32_ERROR(status);
    return registeredGuid;
}
