﻿// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once
#include "Microsoft.Windows.PushNotifications.PushNotificationChannel.g.h"
#include <PushNotificationsLRP_h.h>
#include "pch.h"
#include <iostream>
namespace winrt::Microsoft::Windows::PushNotifications::implementation
{
    typedef winrt::Windows::Foundation::TypedEventHandler<winrt::Microsoft::Windows::PushNotifications::PushNotificationChannel, winrt::Microsoft::Windows::PushNotifications::PushNotificationReceivedEventArgs> PushNotificationHandler;

    struct PushNotificationChannel : PushNotificationChannelT<PushNotificationChannel, IWpnForegroundSink>
    {
        PushNotificationChannel(winrt::Windows::Networking::PushNotifications::PushNotificationChannel const& channel);
        PushNotificationChannel(winrt::Microsoft::Windows::PushNotifications::PushNotificationChannel const& channel);
        ~PushNotificationChannel() noexcept;

        winrt::Windows::Foundation::Uri Uri();
        winrt::Windows::Foundation::DateTime ExpirationTime();
        void Close();

        winrt::event_token PushReceived(winrt::Windows::Foundation::TypedEventHandler<Microsoft::Windows::PushNotifications::PushNotificationChannel, Microsoft::Windows::PushNotifications::PushNotificationReceivedEventArgs> handler);
        void PushReceived(winrt::event_token const& token) noexcept; 

        void WpnForegroundInvoke(byte* payload, ULONG length);

        void TriggerForeground();

        // From IWpnForegroundSink
        HRESULT __stdcall InvokeAll(byte* start, ULONG length) noexcept final try
        {
            std::vector<uint8_t> vec;
            for (int i = 0; (ULONG) i < length; i++)
            {
                vec.push_back(start[i]);
            }
            com_array<uint8_t> arr{ start, start + (length * sizeof(uint8_t)) };
            this->WpnForegroundInvoke(start, length);
            return S_OK;
        }
        CATCH_RETURN();

    private:
        const winrt::Windows::Networking::PushNotifications::PushNotificationChannel m_channel{ nullptr };
        bool m_isBIAvailable;
        winrt::event<PushNotificationHandler> m_foregroundHandlers;


    };
}
namespace winrt::Microsoft::Windows::PushNotifications::factory_implementation
{
    struct PushNotificationChannel : PushNotificationChannelT<PushNotificationChannel, implementation::PushNotificationChannel>
    {
    };
}
