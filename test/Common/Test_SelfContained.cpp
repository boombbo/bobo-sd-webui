﻿// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"

namespace TB = ::Test::Bootstrap;
namespace TP = ::Test::Packages;

namespace Test::Common
{
    class SelfContainedTests
    {
    public:
        BEGIN_TEST_CLASS(SelfContainedTests)
            TEST_CLASS_PROPERTY(L"IsolationLevel", L"Method")
            TEST_CLASS_PROPERTY(L"ThreadingModel", L"MTA")
            TEST_CLASS_PROPERTY(L"RunFixtureAs:Class", L"RestrictedUser")
        END_TEST_CLASS()

        TEST_CLASS_SETUP(ClassSetup)
        {
            ::TB::SetupPackages();
            return true;
        }

        TEST_CLASS_CLEANUP(ClassCleanup)
        {
            ::TB::CleanupPackages();
            return true;
        }

        TEST_METHOD(Unpackaged_SelfContained_Yes)
        {
            ::TB::SetupBootstrap();
            auto cleanup = wil::scope_exit([&] {
                ::WindowsAppRuntime::SelfContained::TestShutdown();
                ::TB::CleanupBootstrap();
            });
            const auto c_doesNotExistPackageFamilyName{ L"Test.PackageFamilyName.DoesNotExist_1234567890abc" };
            ::WindowsAppRuntime::SelfContained::TestInitialize(c_doesNotExistPackageFamilyName);

            VERIFY_IS_TRUE(::WindowsAppRuntime::SelfContained::IsSelfContained());
        }

        TEST_METHOD(Unpackaged_SelfContained_No)
        {
            {
                ::TB::SetupBootstrap();
                auto cleanup = wil::scope_exit([&]{
                    ::WindowsAppRuntime::SelfContained::TestShutdown();
                    ::TB::CleanupBootstrap();
                });
                ::WindowsAppRuntime::SelfContained::TestInitialize(::TP::WindowsAppRuntimeFramework::c_PackageFamilyName);

                VERIFY_IS_FALSE(::WindowsAppRuntime::SelfContained::IsSelfContained());
            }
        }

        TEST_METHOD(Unpackaged_SelfContained)
        {
            ::TB::SetupBootstrap();
            auto cleanup = wil::scope_exit([&] {
                ::WindowsAppRuntime::SelfContained::TestShutdown();
                ::TB::CleanupBootstrap();
            });
            const auto c_doesNotExistPackageFamilyName{ L"Test.PackageFamilyName.DoesNotExist_1234567890abc" };
            ::WindowsAppRuntime::SelfContained::TestInitialize(c_doesNotExistPackageFamilyName);
            VERIFY_IS_TRUE(::WindowsAppRuntime::SelfContained::IsSelfContained());

            ::WindowsAppRuntime::SelfContained::TestInitialize(::TP::WindowsAppRuntimeFramework::c_PackageFamilyName);
            VERIFY_IS_FALSE(::WindowsAppRuntime::SelfContained::IsSelfContained());
            ::WindowsAppRuntime::SelfContained::TestInitialize(c_doesNotExistPackageFamilyName);

            VERIFY_IS_TRUE(::WindowsAppRuntime::SelfContained::IsSelfContained());
        }
    };
}
