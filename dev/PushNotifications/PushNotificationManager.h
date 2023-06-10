﻿// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once
#include "Microsoft.Windows.PushNotifications.PushNotificationManager.g.h"

namespace winrt::Microsoft::Windows::PushNotifications::implementation
{
    struct PushNotificationManager
    {
        PushNotificationManager() = delete;

        static Microsoft::Windows::PushNotifications::PushNotificationRegistrationToken RegisterActivator(Microsoft::Windows::PushNotifications::PushNotificationActivationInfo const& details);
        static void UnregisterActivator(Microsoft::Windows::PushNotifications::PushNotificationRegistrationToken const& token, Microsoft::Windows::PushNotifications::PushNotificationRegistrationOptions const& options);

        static winrt::Windows::Foundation::IAsyncOperationWithProgress<winrt::Microsoft::Windows::PushNotifications::PushNotificationCreateChannelResult, winrt::Microsoft::Windows::PushNotifications::PushNotificationCreateChannelStatus> CreateChannelAsync(const winrt::guid &remoteId);

        static uint32_t GetStringLengthFromComServer(const winrt::hstring& inputString);

        static uint32_t ShutdownPlatformManually();

        static uint32_t TriggerForeground();
        static bool IsActivatorSupported(Microsoft::Windows::PushNotifications::PushNotificationRegistrationOptions const& options);

    private:
        static bool IsChannelRequestRetryable(const winrt::hresult& hrException);
        static bool IsBackgroundTaskBuilderAvailable();
    };
}
namespace winrt::Microsoft::Windows::PushNotifications::factory_implementation
{
    struct PushNotificationManager : PushNotificationManagerT<PushNotificationManager, implementation::PushNotificationManager>
    {
    };
}
