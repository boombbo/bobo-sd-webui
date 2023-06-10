﻿// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

// THIS FILE IS AUTOMATICALLY GENERATED; DO NOT EDIT IT

// INPUT FILE: TerminalVelocityFeatures-Test.xml

// Feature constants
//DISABLED: WINDOWSAPPSDK_FEATURE_ALWAYSDISABLED_ENABLED
//DISABLED: WINDOWSAPPSDK_FEATURE_ALWAYSDISABLED_ALWAYSENABLEDCHANNELS_EXPERIMENTAL_ENABLED
//DISABLED: WINDOWSAPPSDK_FEATURE_ALWAYSDISABLED_ALWAYSENABLEDCHANNELS_EXPERIMENTALPREVIEW_ENABLED
//DISABLED: WINDOWSAPPSDK_FEATURE_ALWAYSDISABLED_ALWAYSENABLEDCHANNELS_EXPERIMENTALPREVIEWSTABLE_ENABLED
//DISABLED: WINDOWSAPPSDK_FEATURE_ALWAYSDISABLED_ALWAYSENABLEDCHANNELS_EXPERIMENTALPREVIEWWINDOWSINBOX_ENABLED
//DISABLED: WINDOWSAPPSDK_FEATURE_ALWAYSDISABLED_ALWAYSENABLEDCHANNELS_EXPERIMENTALSTABLE_ENABLED
//DISABLED: WINDOWSAPPSDK_FEATURE_ALWAYSDISABLED_ALWAYSENABLEDCHANNELS_EXPERIMENTALSTABLEWINDOWSINBOX_ENABLED
//DISABLED: WINDOWSAPPSDK_FEATURE_ALWAYSDISABLED_ALWAYSENABLEDCHANNELS_EXPERIMENTALWINDOWSINBOX_ENABLED
//DISABLED: WINDOWSAPPSDK_FEATURE_ALWAYSDISABLED_ALWAYSENABLEDCHANNELS_PREVIEW_ENABLED
//DISABLED: WINDOWSAPPSDK_FEATURE_ALWAYSDISABLED_ALWAYSENABLEDCHANNELS_PREVIEWSTABLEWINDOWSINBOX_ENABLED
//DISABLED: WINDOWSAPPSDK_FEATURE_ALWAYSDISABLED_ALWAYSENABLEDCHANNELS_STABLE_ENABLED
//DISABLED: WINDOWSAPPSDK_FEATURE_ALWAYSDISABLED_ALWAYSENABLEDCHANNELS_WINDOWSINBOX_ENABLED
//DISABLED: WINDOWSAPPSDK_FEATURE_ALWAYSDISABLED_ID_ENABLED
#Define WINDOWSAPPSDK_FEATURE_ALWAYSENABLED_ENABLED
#Define WINDOWSAPPSDK_FEATURE_ALWAYSENABLED_ALWAYSDISABLEDCHANNELS_EXPERIMENTAL_ENABLED
#Define WINDOWSAPPSDK_FEATURE_ALWAYSENABLED_ALWAYSDISABLEDCHANNELS_EXPERIMENTALPREVIEW_ENABLED
//DISABLED: WINDOWSAPPSDK_FEATURE_ALWAYSENABLED_ALWAYSDISABLEDCHANNELS_EXPERIMENTALPREVIEWSTABLE_ENABLED
#Define WINDOWSAPPSDK_FEATURE_ALWAYSENABLED_ALWAYSDISABLEDCHANNELS_EXPERIMENTALPREVIEWWINDOWSINBOX_ENABLED
//DISABLED: WINDOWSAPPSDK_FEATURE_ALWAYSENABLED_ALWAYSDISABLEDCHANNELS_EXPERIMENTALSTABLE_ENABLED
//DISABLED: WINDOWSAPPSDK_FEATURE_ALWAYSENABLED_ALWAYSDISABLEDCHANNELS_EXPERIMENTALSTABLEWINDOWSINBOX_ENABLED
#Define WINDOWSAPPSDK_FEATURE_ALWAYSENABLED_ALWAYSDISABLEDCHANNELS_EXPERIMENTALWINDOWSINBOX_ENABLED
#Define WINDOWSAPPSDK_FEATURE_ALWAYSENABLED_ALWAYSDISABLEDCHANNELS_PREVIEW_ENABLED
//DISABLED: WINDOWSAPPSDK_FEATURE_ALWAYSENABLED_ALWAYSDISABLEDCHANNELS_PREVIEWSTABLEWINDOWSINBOX_ENABLED
//DISABLED: WINDOWSAPPSDK_FEATURE_ALWAYSENABLED_ALWAYSDISABLEDCHANNELS_STABLE_ENABLED
#Define WINDOWSAPPSDK_FEATURE_ALWAYSENABLED_ALWAYSDISABLEDCHANNELS_WINDOWSINBOX_ENABLED
#Define WINDOWSAPPSDK_FEATURE_ALWAYSENABLED_ID_ENABLED

namespace Test.TerminalVelocity.Features
{

public static class Feature_AlwaysDisabled
{
#if WINDOWSAPPSDK_FEATURE_ALWAYSDISABLED_ENABLED
    public const bool IsEnabled = true;
#else
    public const bool IsEnabled = false;
#endif
};

public static class Feature_AlwaysDisabled_AlwaysEnabledChannels_Experimental
{
#if WINDOWSAPPSDK_FEATURE_ALWAYSDISABLED_ALWAYSENABLEDCHANNELS_EXPERIMENTAL_ENABLED
    public const bool IsEnabled = true;
#else
    public const bool IsEnabled = false;
#endif
};

public static class Feature_AlwaysDisabled_AlwaysEnabledChannels_ExperimentalPreview
{
#if WINDOWSAPPSDK_FEATURE_ALWAYSDISABLED_ALWAYSENABLEDCHANNELS_EXPERIMENTALPREVIEW_ENABLED
    public const bool IsEnabled = true;
#else
    public const bool IsEnabled = false;
#endif
};

public static class Feature_AlwaysDisabled_AlwaysEnabledChannels_ExperimentalPreviewStable
{
#if WINDOWSAPPSDK_FEATURE_ALWAYSDISABLED_ALWAYSENABLEDCHANNELS_EXPERIMENTALPREVIEWSTABLE_ENABLED
    public const bool IsEnabled = true;
#else
    public const bool IsEnabled = false;
#endif
};

public static class Feature_AlwaysDisabled_AlwaysEnabledChannels_ExperimentalPreviewWindowsInbox
{
#if WINDOWSAPPSDK_FEATURE_ALWAYSDISABLED_ALWAYSENABLEDCHANNELS_EXPERIMENTALPREVIEWWINDOWSINBOX_ENABLED
    public const bool IsEnabled = true;
#else
    public const bool IsEnabled = false;
#endif
};

public static class Feature_AlwaysDisabled_AlwaysEnabledChannels_ExperimentalStable
{
#if WINDOWSAPPSDK_FEATURE_ALWAYSDISABLED_ALWAYSENABLEDCHANNELS_EXPERIMENTALSTABLE_ENABLED
    public const bool IsEnabled = true;
#else
    public const bool IsEnabled = false;
#endif
};

public static class Feature_AlwaysDisabled_AlwaysEnabledChannels_ExperimentalStableWindowsInbox
{
#if WINDOWSAPPSDK_FEATURE_ALWAYSDISABLED_ALWAYSENABLEDCHANNELS_EXPERIMENTALSTABLEWINDOWSINBOX_ENABLED
    public const bool IsEnabled = true;
#else
    public const bool IsEnabled = false;
#endif
};

public static class Feature_AlwaysDisabled_AlwaysEnabledChannels_ExperimentalWindowsInbox
{
#if WINDOWSAPPSDK_FEATURE_ALWAYSDISABLED_ALWAYSENABLEDCHANNELS_EXPERIMENTALWINDOWSINBOX_ENABLED
    public const bool IsEnabled = true;
#else
    public const bool IsEnabled = false;
#endif
};

public static class Feature_AlwaysDisabled_AlwaysEnabledChannels_Preview
{
#if WINDOWSAPPSDK_FEATURE_ALWAYSDISABLED_ALWAYSENABLEDCHANNELS_PREVIEW_ENABLED
    public const bool IsEnabled = true;
#else
    public const bool IsEnabled = false;
#endif
};

public static class Feature_AlwaysDisabled_AlwaysEnabledChannels_PreviewStableWindowsInbox
{
#if WINDOWSAPPSDK_FEATURE_ALWAYSDISABLED_ALWAYSENABLEDCHANNELS_PREVIEWSTABLEWINDOWSINBOX_ENABLED
    public const bool IsEnabled = true;
#else
    public const bool IsEnabled = false;
#endif
};

public static class Feature_AlwaysDisabled_AlwaysEnabledChannels_Stable
{
#if WINDOWSAPPSDK_FEATURE_ALWAYSDISABLED_ALWAYSENABLEDCHANNELS_STABLE_ENABLED
    public const bool IsEnabled = true;
#else
    public const bool IsEnabled = false;
#endif
};

public static class Feature_AlwaysDisabled_AlwaysEnabledChannels_WindowsInbox
{
#if WINDOWSAPPSDK_FEATURE_ALWAYSDISABLED_ALWAYSENABLEDCHANNELS_WINDOWSINBOX_ENABLED
    public const bool IsEnabled = true;
#else
    public const bool IsEnabled = false;
#endif
};

public static class Feature_AlwaysDisabled_Id
{
#if WINDOWSAPPSDK_FEATURE_ALWAYSDISABLED_ID_ENABLED
    public const bool IsEnabled = true;
#else
    public const bool IsEnabled = false;
#endif
};

public static class Feature_AlwaysEnabled
{
#if WINDOWSAPPSDK_FEATURE_ALWAYSENABLED_ENABLED
    public const bool IsEnabled = true;
#else
    public const bool IsEnabled = false;
#endif
};

public static class Feature_AlwaysEnabled_AlwaysDisabledChannels_Experimental
{
#if WINDOWSAPPSDK_FEATURE_ALWAYSENABLED_ALWAYSDISABLEDCHANNELS_EXPERIMENTAL_ENABLED
    public const bool IsEnabled = true;
#else
    public const bool IsEnabled = false;
#endif
};

public static class Feature_AlwaysEnabled_AlwaysDisabledChannels_ExperimentalPreview
{
#if WINDOWSAPPSDK_FEATURE_ALWAYSENABLED_ALWAYSDISABLEDCHANNELS_EXPERIMENTALPREVIEW_ENABLED
    public const bool IsEnabled = true;
#else
    public const bool IsEnabled = false;
#endif
};

public static class Feature_AlwaysEnabled_AlwaysDisabledChannels_ExperimentalPreviewStable
{
#if WINDOWSAPPSDK_FEATURE_ALWAYSENABLED_ALWAYSDISABLEDCHANNELS_EXPERIMENTALPREVIEWSTABLE_ENABLED
    public const bool IsEnabled = true;
#else
    public const bool IsEnabled = false;
#endif
};

public static class Feature_AlwaysEnabled_AlwaysDisabledChannels_ExperimentalPreviewWindowsInbox
{
#if WINDOWSAPPSDK_FEATURE_ALWAYSENABLED_ALWAYSDISABLEDCHANNELS_EXPERIMENTALPREVIEWWINDOWSINBOX_ENABLED
    public const bool IsEnabled = true;
#else
    public const bool IsEnabled = false;
#endif
};

public static class Feature_AlwaysEnabled_AlwaysDisabledChannels_ExperimentalStable
{
#if WINDOWSAPPSDK_FEATURE_ALWAYSENABLED_ALWAYSDISABLEDCHANNELS_EXPERIMENTALSTABLE_ENABLED
    public const bool IsEnabled = true;
#else
    public const bool IsEnabled = false;
#endif
};

public static class Feature_AlwaysEnabled_AlwaysDisabledChannels_ExperimentalStableWindowsInbox
{
#if WINDOWSAPPSDK_FEATURE_ALWAYSENABLED_ALWAYSDISABLEDCHANNELS_EXPERIMENTALSTABLEWINDOWSINBOX_ENABLED
    public const bool IsEnabled = true;
#else
    public const bool IsEnabled = false;
#endif
};

public static class Feature_AlwaysEnabled_AlwaysDisabledChannels_ExperimentalWindowsInbox
{
#if WINDOWSAPPSDK_FEATURE_ALWAYSENABLED_ALWAYSDISABLEDCHANNELS_EXPERIMENTALWINDOWSINBOX_ENABLED
    public const bool IsEnabled = true;
#else
    public const bool IsEnabled = false;
#endif
};

public static class Feature_AlwaysEnabled_AlwaysDisabledChannels_Preview
{
#if WINDOWSAPPSDK_FEATURE_ALWAYSENABLED_ALWAYSDISABLEDCHANNELS_PREVIEW_ENABLED
    public const bool IsEnabled = true;
#else
    public const bool IsEnabled = false;
#endif
};

public static class Feature_AlwaysEnabled_AlwaysDisabledChannels_PreviewStableWindowsInbox
{
#if WINDOWSAPPSDK_FEATURE_ALWAYSENABLED_ALWAYSDISABLEDCHANNELS_PREVIEWSTABLEWINDOWSINBOX_ENABLED
    public const bool IsEnabled = true;
#else
    public const bool IsEnabled = false;
#endif
};

public static class Feature_AlwaysEnabled_AlwaysDisabledChannels_Stable
{
#if WINDOWSAPPSDK_FEATURE_ALWAYSENABLED_ALWAYSDISABLEDCHANNELS_STABLE_ENABLED
    public const bool IsEnabled = true;
#else
    public const bool IsEnabled = false;
#endif
};

public static class Feature_AlwaysEnabled_AlwaysDisabledChannels_WindowsInbox
{
#if WINDOWSAPPSDK_FEATURE_ALWAYSENABLED_ALWAYSDISABLEDCHANNELS_WINDOWSINBOX_ENABLED
    public const bool IsEnabled = true;
#else
    public const bool IsEnabled = false;
#endif
};

public static class Feature_AlwaysEnabled_Id
{
#if WINDOWSAPPSDK_FEATURE_ALWAYSENABLED_ID_ENABLED
    public const bool IsEnabled = true;
#else
    public const bool IsEnabled = false;
#endif
};

} // namespace Test.TerminalVelocity.Features
