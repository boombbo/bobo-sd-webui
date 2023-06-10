﻿// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once
#include "TestSetupAndTeardownHelper.h"

namespace WindowsAppSDKEnvironmentManagerTests
{
    class EnvironmentManagerWin32Tests {
        BEGIN_TEST_CLASS(EnvironmentManagerWin32Tests)
            TEST_CLASS_PROPERTY(L"ThreadingModel", L"MTA")
            TEST_CLASS_PROPERTY(L"RunFixtureAs:Method", L"ElevatedUser")
            TEST_CLASS_PROPERTY(L"RunAs", L"RestrictedUser")
        END_TEST_CLASS()


        TEST_CLASS_SETUP(ClassInit)
        {
            ::Test::Bootstrap::Setup();
            return true;
        }

        TEST_CLASS_CLEANUP(ClassUninit)
        {
            ::Test::Bootstrap::Cleanup();
            return true;
        }

        TEST_METHOD_SETUP(WriteEVs)
        {
            VERIFY_IS_TRUE(TP::IsPackageRegistered_WindowsAppRuntimeFramework());
            VERIFY_IS_TRUE(TP::IsPackageRegistered_DynamicDependencyDataStore());
            VERIFY_IS_TRUE(TP::IsPackageRegistered_DynamicDependencyLifetimeManager());

            UserSetup();
            MachineSetup();
            return true;
        }

        TEST_METHOD_CLEANUP(RemoveEVs)
        {
            UserCleanup();
            MachineCleanup();
            return true;
        }

        TEST_METHOD(TestGetForProcess);
        TEST_METHOD(TestGetForUser);
        TEST_METHOD(TestGetForMachine);

        TEST_METHOD(TestGetEnvironmentVariablesForProcess);
        TEST_METHOD(TestGetEnvironmentVariablesForUser);
        TEST_METHOD(TestGetEnvironmentVariablesForMachine);

        TEST_METHOD(TestGetEnvironmentVariableForProcess);
        TEST_METHOD(TestGetEnvironmentVariableForUser);
        TEST_METHOD(TestGetEnvironmentVariableForMachine);

        TEST_METHOD(TestSetEnvironmentVariableForProcess);
        TEST_METHOD(TestSetEnvironmentVariableForUser);
        TEST_METHOD(TestSetEnvironmentVariableForMachine);

        TEST_METHOD(TestAppendToPathForProcess);
        TEST_METHOD(TestAppendToPathForUser);
        TEST_METHOD(TestAppendToPathForMachine);

        TEST_METHOD(TestRemoveFromPathForProcess);
        TEST_METHOD(TestRemoveFromPathForUser);
        TEST_METHOD(TestRemoveFromPathForMachine);

        TEST_METHOD(TestAppendToPathExtForProcess);
        TEST_METHOD(TestAppendToPathExtForUser);
        TEST_METHOD(TestAppendToPathExtForMachine);

        TEST_METHOD(TestRemoveFromPathExtForProcess);
        TEST_METHOD(TestRemoveFromPathExtForUser);
        TEST_METHOD(TestRemoveFromPathExtForMachine);
    };
}
