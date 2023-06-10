﻿// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once
#include "TestSetupAndTeardownHelper.h"
#include <TerminalVelocityFeatures-EnvironmentManager.h>

namespace WindowsAppSDKEnvironmentManagerTests
{
    class EnvironmentManagerUWPTests {
        BEGIN_TEST_CLASS(EnvironmentManagerUWPTests)
            TEST_CLASS_PROPERTY(L"RunAs", L"UAP")
            TEST_CLASS_PROPERTY(L"UAP:AppxManifest", L"AppxManifest.pkg.xml")
            TEST_CLASS_PROPERTY(L"RunFixtureAs:Class", L"RestrictedUser")
            TEST_CLASS_PROPERTY(L"RunFixtureAs:Method", L"ElevatedUser")
            END_TEST_CLASS()

        TEST_CLASS_SETUP(ClassInit)
        {
            if (!::Microsoft::Windows::System::Feature_EnvironmentManager::IsEnabled())
            {
                WEX::Logging::Log::Result(WEX::Logging::TestResults::Skipped, L"Environment Manager is experimental.");
                return false;
            }

            ::Test::Bootstrap::Setup();
            return true;
        }

        TEST_CLASS_CLEANUP(ClassUninit)
        {
            ::Test::Bootstrap::Cleanup();
            return true;
        }

        TEST_METHOD_SETUP(UWPWriteEVs)
        {
            VERIFY_IS_TRUE(TP::IsPackageRegistered_WindowsAppRuntimeFramework());
            VERIFY_IS_TRUE(TP::IsPackageRegistered_DynamicDependencyDataStore());
            VERIFY_IS_TRUE(TP::IsPackageRegistered_DynamicDependencyLifetimeManager());

            UserSetup();
            MachineSetup();
            return true;
        }

        TEST_METHOD_CLEANUP(UWPRemoveEVs)
        {
            UserCleanup();
            MachineCleanup();
            return true;
        }

        TEST_METHOD(UWPTestGetForProcess);
        TEST_METHOD(UWPTestGetForUser);
        TEST_METHOD(UWPTestGetForMachine);

        TEST_METHOD(UWPTestGetEnvironmentVariablesForProcess);
        TEST_METHOD(UWPTestGetEnvironmentVariablesForUser);
        TEST_METHOD(UWPTestGetEnvironmentVariablesForMachine);

        TEST_METHOD(UWPTestGetEnvironmentVariableForProcess);
        TEST_METHOD(UWPTestGetEnvironmentVariableForUser);
        TEST_METHOD(UWPTestGetEnvironmentVariableForMachine);

        TEST_METHOD(UWPTestSetEnvironmentVariableForProcess);
        TEST_METHOD(UWPTestSetEnvironmentVariableForUser);
        TEST_METHOD(UWPTestSetEnvironmentVariableForMachine);

        TEST_METHOD(UWPTestAppendToPathForProcess);
        TEST_METHOD(UWPTestAppendToPathForUser);
        TEST_METHOD(UWPTestAppendToPathForMachine);

        TEST_METHOD(UWPTestRemoveFromPathForProcess);
        TEST_METHOD(UWPTestRemoveFromPathForUser);
        TEST_METHOD(UWPTestRemoveFromPathForMachine);

        TEST_METHOD(UWPTestAppendToPathExtForProcess);
        TEST_METHOD(UWPTestAppendToPathExtForUser);
        TEST_METHOD(UWPTestAppendToPathExtForMachine);

        TEST_METHOD(UWPTestRemoveFromPathExtForProcess);
        TEST_METHOD(UWPTestRemoveFromPathExtForUser);
        TEST_METHOD(UWPTestRemoveFromPathExtForMachine);
    };
}
