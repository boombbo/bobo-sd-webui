﻿// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

// THIS FILE IS AUTOMATICALLY GENERATED; DO NOT EDIT IT

// INPUT FILE: dev\common\TerminalVelocityFeatures-EnvironmentManager.xml
// OPTIONS: -Channel Experimental -Language C++ -Namespace Microsoft.Windows.System

// Feature constants
#define WINDOWSAPPRUNTIME_MICROSOFT_WINDOWS_SYSTEM_FEATURE_ENVIRONMENTMANAGER_ENABLED 1

#if defined(__cplusplus)

namespace Microsoft::Windows::System
{

__pragma(detect_mismatch("ODR_violation_WINDOWSAPPRUNTIME_MICROSOFT_WINDOWS_SYSTEM_FEATURE_ENVIRONMENTMANAGER_ENABLED_mismatch", "AlwaysEnabled"))
struct Feature_EnvironmentManager
{
    static constexpr bool IsEnabled() { return WINDOWSAPPRUNTIME_MICROSOFT_WINDOWS_SYSTEM_FEATURE_ENVIRONMENTMANAGER_ENABLED == 1; }
};

} // namespace Microsoft.Windows.System

#endif // defined(__cplusplus)
