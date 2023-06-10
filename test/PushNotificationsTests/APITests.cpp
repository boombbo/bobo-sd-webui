﻿// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"
#include <testdef.h>
#include "Shared.h"
#include "MockBackgroundTaskInstance.h"
#include "winrt/Windows.Foundation.h"

using namespace WEX::Common;
using namespace WEX::Logging;
using namespace WEX::TestExecution;

namespace winrt
{
    namespace Microsoft
    {
        using namespace Windows::AppLifecycle;
        using namespace Windows::PushNotifications;
    }

    namespace Windows
    {
        using namespace ApplicationModel;
        using namespace ApplicationModel::Activation;
        using namespace ApplicationModel::Background;
        using namespace Foundation;
        using namespace Foundation::Collections;
        using namespace Management::Deployment;
        using namespace Storage;
        using namespace System;
    }
}
// TODO: Write Register/Unregister tests that utilize the Assoc APIs to validate results.

namespace Test::PushNotifications
{
    class APITests
    {
    private:
        wil::unique_event m_failed;

        const std::wstring c_testPackageFile = g_deploymentDir + L"MSIXPackager_1.0.0.0_x64_Debug.msixbundle";
        const std::wstring c_testPackageCertFile = g_deploymentDir + L"MSIXPackager_1.0.0.0_x64_Debug.cer";
        const std::wstring c_testPackageFullName = L"PushNotificationsWin32App_1.0.0.0_x64__9eg3vg7cmy3rj";

    public:
        BEGIN_TEST_CLASS(APITests)
            TEST_CLASS_PROPERTY(L"Description", L"Project Reunion Push Notifications test")
            TEST_CLASS_PROPERTY(L"IsolationLevel", L"Method")
            TEST_CLASS_PROPERTY(L"ThreadingModel", L"MTA")
            TEST_CLASS_PROPERTY(L"RunFixtureAs:Class", L"ElevatedUser")
        END_TEST_CLASS()

        TEST_CLASS_SETUP(ClassInit)
        {
            ::Test::Bootstrap::SetupPackages();
            try
            {
                RunCertUtil(c_testPackageCertFile);
                InstallPackage(c_testPackageFile);
            }
            catch (...)
            {
                ::Test::Bootstrap::CleanupPackages();
                return false;
            }
            return true;
        }

        TEST_CLASS_CLEANUP(ClassUninit)
        {
            ::Test::Bootstrap::CleanupPackages();
            try
            {
                UninstallPackage(c_testPackageFullName);
                RunCertUtil(c_testPackageCertFile);
            }
            catch (...)
            {
                return false;
            }
            return true;
        }

        TEST_METHOD_SETUP(MethodInit)
        {
            VERIFY_IS_TRUE(TP::IsPackageRegistered_ProjectReunionFramework());
            m_failed = CreateTestEvent(c_testFailureEventName);

            return true;
        }

        TEST_METHOD_CLEANUP(MethodUninit)
        {
            VERIFY_IS_TRUE(TP::IsPackageRegistered_ProjectReunionFramework());

            return true;
        }

        
        TEST_METHOD(ChannelRequestUsingNullRemoteId)
        {
            wil::unique_event event = CreateTestEvent(c_testProtocolScheme_Packaged);

            // This is associated protocol for the MSIX installed app for launch.
            // Use the ://path to define the component you want to test.
            winrt::Windows::Uri launchUri{ c_testProtocolScheme_Packaged + L"://ChannelRequestUsingNullRemoteId" };
            auto launchResult = winrt::Windows::Launcher::LaunchUriAsync(launchUri).get();
            VERIFY_IS_TRUE(launchResult);

            WaitForEvent(event, m_failed);
        }
        /*
        TEST_METHOD(ChannelRequestUsingRemoteId)
        {
            wil::unique_event event = CreateTestEvent(c_testProtocolScheme_Packaged);

            // This is associated protocol for the MSIX installed app for launch.
            // Use the ://path to define the component you want to test.
            winrt::Windows::Uri launchUri{ c_testProtocolScheme_Packaged + L"://ChannelRequestUsingRemoteId" };
            auto launchResult = winrt::Windows::Launcher::LaunchUriAsync(launchUri).get();
            VERIFY_IS_TRUE(launchResult);

            WaitForEvent(event, m_failed);
        }

        TEST_METHOD(MultipleChannelRequestUsingSameRemoteId)
        {
            wil::unique_event event = CreateTestEvent(c_testProtocolScheme_Packaged);

            // This is associated protocol for the MSIX installed app for launch.
            // Use the ://path to define the component you want to test.
            winrt::Windows::Uri launchUri{ c_testProtocolScheme_Packaged + L"://MultipleChannelRequestUsingSameRemoteId" };
            auto launchResult = winrt::Windows::Launcher::LaunchUriAsync(launchUri).get();
            VERIFY_IS_TRUE(launchResult);

            WaitForEvent(event, m_failed);
        }

        TEST_METHOD(MultipleChannelRequestUsingMultipleRemoteId)
        {
            wil::unique_event event = CreateTestEvent(c_testProtocolScheme_Packaged);

            // This is associated protocol for the MSIX installed app for launch.
            // Use the ://path to define the component you want to test.
            winrt::Windows::Uri launchUri{ c_testProtocolScheme_Packaged + L"://MultipleChannelRequestUsingMultipleRemoteId" };
            auto launchResult = winrt::Windows::Launcher::LaunchUriAsync(launchUri).get();
            VERIFY_IS_TRUE(launchResult);

            WaitForEvent(event, m_failed);
        }

        TEST_METHOD(ThreeChannelRequestUsingSameRemoteId)
        {
            wil::unique_event event = CreateTestEvent(c_testProtocolScheme_Packaged);

            // This is associated protocol for the MSIX installed app for launch.
            // Use the ://path to define the component you want to test.
            winrt::Windows::Uri launchUri{ c_testProtocolScheme_Packaged + L"://ThreeChannelRequestUsingSameRemoteId" };
            auto launchResult = winrt::Windows::Launcher::LaunchUriAsync(launchUri).get();
            VERIFY_IS_TRUE(launchResult);

            WaitForEvent(event, m_failed);
        }
        */
        TEST_METHOD(BackgroundActivation)
        {
            wil::unique_event event = CreateTestEvent(c_testProtocolScheme_Packaged);
            auto LocalBackgroundTask = winrt::create_instance<winrt::Windows::ApplicationModel::Background::IBackgroundTask>(c_comServerId, CLSCTX_ALL);
            auto mockBackgroundTaskInstance = winrt::make<MockBackgroundTaskInstance>();
            LocalBackgroundTask.Run(mockBackgroundTaskInstance);
            WaitForEvent(event, m_failed);
        }

        TEST_METHOD(RegisterActivator)
        {
            wil::unique_event event = CreateTestEvent(c_testProtocolScheme_Packaged);
            winrt::Windows::Uri launchUri{ c_testProtocolScheme_Packaged + L"://RegisterActivator" };
            auto launchResult = winrt::Windows::Launcher::LaunchUriAsync(launchUri).get();
            VERIFY_IS_TRUE(launchResult);

            WaitForEvent(event, m_failed);
        }

        TEST_METHOD(UnregisterActivator)
        {
            wil::unique_event event = CreateTestEvent(c_testProtocolScheme_Packaged);
            winrt::Windows::Uri launchUri{ c_testProtocolScheme_Packaged + L"://UnregisterActivator" };
            auto launchResult = winrt::Windows::Launcher::LaunchUriAsync(launchUri).get();
            VERIFY_IS_TRUE(launchResult);

            WaitForEvent(event, m_failed);
        }
    };
}
