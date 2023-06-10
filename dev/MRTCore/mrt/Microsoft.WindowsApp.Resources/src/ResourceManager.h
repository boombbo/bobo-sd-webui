﻿// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once
#include "ResourceManager.g.h"

namespace winrt::Microsoft::WindowsApp::Resources::implementation
{

struct ResourceManager : ResourceManagerT<ResourceManager>
{
    ResourceManager();
    ResourceManager(hstring const& fileName);

    Microsoft::WindowsApp::Resources::ResourceMap MainResourceMap();
    Microsoft::WindowsApp::Resources::ResourceContext CreateResourceContext();

    winrt::event_token ResourceNotFound(Windows::Foundation::TypedEventHandler<
                                        Microsoft::WindowsApp::Resources::ResourceManager,
                                        Microsoft::WindowsApp::Resources::ResourceNotFoundEventArgs> const& handler);

    void ResourceNotFound(winrt::event_token const& token) noexcept;

    Microsoft::WindowsApp::Resources::ResourceCandidate HandleResourceNotFound(
        Microsoft::WindowsApp::Resources::ResourceContext context,
        hstring name);

private:
    ~ResourceManager();
    MrmManagerHandle m_resourceManagerHandle = nullptr;
    slim_mutex m_lock;

    winrt::event<winrt::Windows::Foundation::TypedEventHandler<
        Microsoft::WindowsApp::Resources::ResourceManager,
        Microsoft::WindowsApp::Resources::ResourceNotFoundEventArgs>>
        m_resourceNotFound;
};

} // namespace winrt::Microsoft::WindowsApp::Resources::implementation

namespace winrt::Microsoft::WindowsApp::Resources::factory_implementation
{

struct ResourceManager : ResourceManagerT<ResourceManager, implementation::ResourceManager>
{};

} // namespace winrt::Microsoft::WindowsApp::Resources::factory_implementation
