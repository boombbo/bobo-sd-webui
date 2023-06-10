﻿// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once
#include "Microsoft.Windows.PushNotifications.PushNotificationChannel.g.h"
#include <NotificationsLongRunningProcess_h.h>
#include "winrt/Windows.Networking.PushNotifications.h"
#include "externs.h"

namespace winrt::Microsoft::Windows::PushNotifications::implementation
{
    typedef winrt::Windows::Foundation::TypedEventHandler<
        winrt::Microsoft::Windows::PushNotifications::PushNotificationChannel,
        winrt::Microsoft::Windows::PushNotifications::PushNotificationReceivedEventArgs> PushNotificationEventHandler;

    struct PushNotificationChannel : PushNotificationChannelT<PushNotificationChannel, IWpnForegroundSink>
    {
        PushNotificationChannel(winrt::Windows::Networking::PushNotifications::PushNotificationChannel const& channel) : m_channel(channel) {};

        PushNotificationChannel(struct ChannelDetails const& channelInfo) : m_channelInfo(channelInfo) {};

        winrt::Windows::Foundation::Uri Uri();
        winrt::Windows::Foundation::DateTime ExpirationTime();
        void Close();

        winrt::event_token PushReceived(winrt::Windows::Foundation::TypedEventHandler<Microsoft::Windows::PushNotifications::PushNotificationChannel, Microsoft::Windows::PushNotifications::PushNotificationReceivedEventArgs> handler);
        void PushReceived(winrt::event_token const& token) noexcept;

        // IWpnForegroundSink
        HRESULT __stdcall InvokeAll(_In_ ULONG length, _In_ byte* payload, _Out_ BOOL* foregroundHandled) noexcept;

    private:
        bool IsPackagedAppScenario();
        bool IsBackgroundTaskBuilderAvailable();

        const winrt::Windows::Networking::PushNotifications::PushNotificationChannel m_channel{ nullptr };
        const struct ChannelDetails m_channelInfo{};

        winrt::event<PushNotificationEventHandler> m_foregroundHandlers;
        ULONG m_foregroundHandlerCount = 0;
        wil::srwlock m_lock;
    };
}
