﻿// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"

#include <MsixDynamicDependency.h>

#include <Math.Add.h>
#include <Math.Multiply.h>

#include "Test_WinRT.h"

namespace TF = ::Test::FileSystem;
namespace TP = ::Test::Packages;

void Test::DynamicDependency::Test_WinRT::Create_FilePathLifetime_NoExist()
{
    // Setup our dynamic dependencies

    winrt::hstring expectedPackageFullName_WindowsAppSDKFramework{ TP::WindowsAppSDKFramework::c_PackageFullName };
    winrt::hstring expectedPackageFullName_FrameworkMathAdd{ TP::FrameworkMathAdd::c_PackageFullName };

    VerifyPackageInPackageGraph(expectedPackageFullName_WindowsAppSDKFramework, S_OK);
    VerifyPackageNotInPackageGraph(expectedPackageFullName_FrameworkMathAdd, S_OK);
    auto pathEnvironmentVariable{ GetPathEnvironmentVariableMinusWindowsAppSDKFramework() };
    auto packagePath_WindowsAppSDKFramework{ TP::GetPackagePath(expectedPackageFullName_WindowsAppSDKFramework) };
    VerifyPathEnvironmentVariable(packagePath_WindowsAppSDKFramework, pathEnvironmentVariable.c_str());

    // -- Create

    auto lifetimeArtifactPath{ std::filesystem::temp_directory_path() };
    auto lifetimeArtifactFilename{ lifetimeArtifactPath / L"Test-MathAdd-LifetimeArtifact.tmp" };
    std::filesystem::remove(lifetimeArtifactFilename);

    try
    {
        const auto lifetimeKind{ winrt::Microsoft::Windows::ApplicationModel::DynamicDependency::PackageDependencyLifetimeArtifactKind::FilePath };
        auto packageDependency_FrameworkMathAdd{ _Create_FrameworkMathAdd(HRESULT_FROM_WIN32(ERROR_CONTEXT_EXPIRED), lifetimeKind, lifetimeArtifactFilename.c_str()) };
        auto message{ wil::str_printf<wil::unique_process_heap_string>(L"Expected exception (value=0x%X ERROR_CONTEXT_EXPIRED) didn't occur", HRESULT_FROM_WIN32(ERROR_CONTEXT_EXPIRED)) };
        VERIFY_FAIL(message.get());
    }
    catch (const winrt::hresult_error& e)
    {
        constexpr int32_t expectedHR{ HRESULT_FROM_WIN32(ERROR_CONTEXT_EXPIRED) };
        VERIFY_ARE_EQUAL(expectedHR, e.code().value, e.message().c_str());
    }

    VerifyPackageInPackageGraph(expectedPackageFullName_WindowsAppSDKFramework, S_OK);
    VerifyPackageNotInPackageGraph(expectedPackageFullName_FrameworkMathAdd, S_OK);
    VerifyPathEnvironmentVariable(packagePath_WindowsAppSDKFramework, pathEnvironmentVariable.c_str());
}
