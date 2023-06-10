﻿// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
#pragma once

#include <winrt/Windows.Foundation.h>
#include "ActivatedEventArgsBase.h"

namespace winrt::Microsoft::Windows::AppLifecycle::implementation
{
    using namespace winrt::Windows::ApplicationModel::Activation;

    constexpr PCWSTR c_startupTaskContractId = L"Windows.StartupTask";

    class StartupActivatedEventArgs : public winrt::implements<StartupActivatedEventArgs, IStartupTaskActivatedEventArgs,
        ActivatedEventArgsBase, IInternalValueMarshalable>
    {
    public:
        StartupActivatedEventArgs(const winrt::hstring taskId) : m_taskId(taskId)
        {
            m_kind = ActivationKind::StartupTask;
        }

        static winrt::Windows::Foundation::IInspectable Deserialize(winrt::Windows::Foundation::Uri const& uri)
        {
            auto query = uri.QueryParsed();
            auto taskId = query.GetFirstValueByName(L"TaskId");
            return make<StartupActivatedEventArgs>(taskId);
        }

        // IInternalValueMarshalable
        winrt::Windows::Foundation::Uri Serialize()
        {
            auto uri = GenerateEncodedLaunchUri(L"App", c_startupTaskContractId) + L"&TaskId=" + winrt::Windows::Foundation::Uri::EscapeComponent(m_taskId.c_str());
            return winrt::Windows::Foundation::Uri(uri);
        }

        // IStartupTaskActivatedEventArgs
        winrt::hstring TaskId()
        {
            return m_taskId.c_str();
        }

    private:
        winrt::hstring m_taskId;
    };
}

