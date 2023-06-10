﻿// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once
#include "pch.h"
#include <winrt/Windows.Foundation.h>
#include <wrl/implements.h>
#include <wil/resource.h>
#include <AppNotificationActivatedEventArgs.h>
#include <FrameworkUdk/toastnotificationsrt.h>

namespace Microsoft::Windows::AppNotifications::Helpers
{
    const std::wstring c_appIdentifierPath{ LR"(Software\Classes\AppUserModelId\)" };
    const std::wstring c_clsIdPath{ LR"(Software\Classes\CLSID\)" };
    const std::wstring c_quote{ LR"(")" };
    const std::wstring c_backSlash{ LR"(\)" };
    const std::wstring c_notificationActivatedArgument{ L" ----AppNotificationActivated:" };
    const std::wstring c_localMicrosoftFolder{ LR"(\Microsoft\)" };
    const std::wstring c_fileExtensionFormat{ LR"(.png)" };

    struct AppNotificationAssets {
        std::wstring displayName;
        std::wstring iconFilePath;
    };

    inline const int GUID_LENGTH = 39; // GUID + '{' + '}' + '/0'

    inline std::wstring ConvertPathToKey(std::wstring path)
    {
        for (int i = 0; i < path.length(); i++)
        {
            if (path[i] == '\\')
            {
                path[i] = '.';
            }
        }
        return path;
    }

    inline void RegisterValue(wil::unique_hkey const& hKey, PCWSTR const& key, const BYTE* value, DWORD const& valueType, size_t const& size)
    {
        THROW_IF_WIN32_ERROR(RegSetValueExW(hKey.get(), key, 0, valueType, value, static_cast<DWORD>(size)));
    }

    inline wil::unique_hstring safe_make_unique_hstring(PCWSTR str)
    {
        // wil::make_unique_string can't allocate a zero byte memory buffer.
        return (str == nullptr || wcslen(str) == 0) ? wil::unique_hstring() : wil::make_unique_string<wil::unique_hstring>(str);
    }

    std::wstring RetrieveUnpackagedNotificationAppId();

    std::wstring RetrieveNotificationAppId();

    void RegisterComServer(wil::unique_cotaskmem_string const& clsid);

    void UnRegisterComServer(std::wstring const& clsid);

    void UnRegisterNotificationAppIdentifierFromRegistry();

    HRESULT GetActivatorGuid(std::wstring& activatorGuid) noexcept;

    winrt::guid RegisterComActivatorGuidAndAssets();

    void RegisterAssets(std::wstring const& appId, std::wstring const& clsid);

    wil::unique_cotaskmem_string ConvertUtf8StringToWideString(unsigned long length, const BYTE* utf8String);

    winrt::Microsoft::Windows::AppNotifications::AppNotification ToastNotificationFromToastProperties(ABI::Microsoft::Internal::ToastNotifications::INotificationProperties* properties);

    std::wstring GetDisplayNameBasedOnProcessName();

    HRESULT RetrieveAssetsFromProcess(_Out_ Microsoft::Windows::AppNotifications::Helpers::AppNotificationAssets& assets) noexcept;

}
