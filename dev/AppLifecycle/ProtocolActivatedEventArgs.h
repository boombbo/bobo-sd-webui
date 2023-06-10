﻿// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
#pragma once

#include <winrt/Windows.Foundation.h>
#include "ActivatedEventArgsBase.h"

namespace winrt::Microsoft::Windows::AppLifecycle::implementation
{
    using namespace winrt::Windows::Foundation::Collections;
    using namespace winrt::Windows::ApplicationModel::Activation;

    static PCWSTR c_protocolContractId = L"Windows.Protocol";

    class ProtocolActivatedEventArgs : public winrt::implements<ProtocolActivatedEventArgs, ActivatedEventArgsBase, IProtocolActivatedEventArgs,
        IInternalValueMarshalable>
    {
    public:
        ProtocolActivatedEventArgs(const std::wstring& uri) : m_uri(winrt::Windows::Foundation::Uri(uri))
        {
            m_kind = ActivationKind::Protocol;
        }

        static IActivatedEventArgs Deserialize(winrt::Windows::Foundation::Uri const& uri)
        {
            auto query = uri.QueryParsed();
            auto args = query.GetFirstValueByName(L"Uri").c_str();
            return make<ProtocolActivatedEventArgs>(args);
        }

        // IInternalValueMarshalable
        winrt::Windows::Foundation::Uri Serialize()
        {
            auto uri = GenerateEncodedLaunchUri(L"App", c_protocolContractId) + L"&Uri=" + m_uri.AbsoluteUri();
            return winrt::Windows::Foundation::Uri(uri);
        }

        // IProtocolActivatedEventArgs
        winrt::Windows::Foundation::Uri Uri()
        {
            return m_uri;
        }

    private:
        winrt::Windows::Foundation::Uri m_uri{nullptr};
    };
}
