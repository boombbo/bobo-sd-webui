﻿// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once
#include "Microsoft.Windows.PushNotifications.PushNotificationActivationInfo.g.h"

namespace winrt::Microsoft::Windows::PushNotifications::implementation
{
    struct PushNotificationActivationInfo : PushNotificationActivationInfoT<PushNotificationActivationInfo>
    {
        PushNotificationActivationInfo(Microsoft::Windows::PushNotifications::PushNotificationRegistrationOption const& option, winrt::guid const& taskClsid);
        winrt::guid TaskClsid();
        Microsoft::Windows::PushNotifications::PushNotificationRegistrationOption Option();
        winrt::com_array<winrt::Windows::ApplicationModel::Background::IBackgroundCondition> GetConditions();
        void SetConditions(array_view<winrt::Windows::ApplicationModel::Background::IBackgroundCondition const> conditions);

    private:
        const Microsoft::Windows::PushNotifications::PushNotificationRegistrationOption m_option;
        const winrt::guid m_taskClsid;
        winrt::com_array<winrt::Windows::ApplicationModel::Background::IBackgroundCondition> m_backgroundConditions{};
        wil::srwlock m_lock;
    };
}
namespace winrt::Microsoft::Windows::PushNotifications::factory_implementation
{
    struct PushNotificationActivationInfo : PushNotificationActivationInfoT<PushNotificationActivationInfo, implementation::PushNotificationActivationInfo>
    {
    };
}
