﻿#pragma once
#include "pch.h"
#include "CppUnitTest.h"

namespace WindowsAppRuntimeInstallerTests
{
    wil::unique_handle Execute(const std::wstring& command, const std::wstring& args);
    HRESULT RunInstaller(const std::wstring& args=L"");
    void RemovePackage(const std::wstring& packageFullName, bool ignoreFailures=true);
    void TryRemoveProvisionedPackage(const std::wstring& packageFamilyName);
    void RemoveAllPackages(bool ignoreFailures=true);
    bool IsPackageRegistered(const std::wstring& packageFullName);
    winrt::Windows::System::ProcessorArchitecture GetSystemArchitecture();
}
