﻿// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <vector>

class ToastRegistrationManager
{
public:
    ToastRegistrationManager() = default;

    void Add(std::wstring const& appId, std::wstring const& toastGuid);

    void Remove(std::wstring const& appId);

    std::wstring GetToastRegistration(std::wstring const& processName);

private:
    std::unordered_map<std::wstring, std::wstring> m_registrationMap = {};
    wil::srwlock m_lock;
};
