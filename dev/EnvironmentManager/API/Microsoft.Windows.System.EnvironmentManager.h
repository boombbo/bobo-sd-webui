﻿#pragma once
#include "Microsoft.Windows.System.EnvironmentManager.g.h"

using namespace winrt::Windows::Foundation::Collections;

namespace winrt::Microsoft::Windows::System::implementation
{
    struct EnvironmentManager : EnvironmentManagerT<EnvironmentManager>
    {
        enum Scope
        {
            Process = 0,
            User = 1,
            Machine = 2
        };

        EnvironmentManager(Scope const& scope);

        static Microsoft::Windows::System::EnvironmentManager GetForProcess();
        static Microsoft::Windows::System::EnvironmentManager GetForUser();
        static Microsoft::Windows::System::EnvironmentManager GetForMachine();
        static bool IsSupported();
        IMapView<hstring, hstring> GetEnvironmentVariables();
        hstring GetEnvironmentVariable(hstring const& variableName);
        void SetEnvironmentVariable(hstring const& name, hstring const& value);
        void AppendToPath(hstring const& path);
        void RemoveFromPath(hstring const& path);
        void AddExecutableFileExtension(hstring const& pathExt);
        void RemoveExecutableFileExtension(hstring const& pathExt);

        static std::wstring ScopeToString(Scope scope)
        {
            if (scope == Scope::Process)
            {
                return L"process";
            }
            else if (scope == Scope::User)
            {
                return L"user";
            }
            else if (scope == Scope::Machine)
            {
                return L"machine";
            }
            else
            {
                THROW_HR_MSG(E_INVALIDARG, "Scope is not either process, user, or machine");
            }
        }

    private:
        Scope m_Scope{};

        PCWSTR c_UserEvRegLocation{ L"Environment" };
        PCWSTR c_MachineEvRegLocation{ L"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment" };
        PCWSTR c_PathName{ L"PATH" };
        PCWSTR c_PathExtName{ L"PATHEXT" };

        static bool s_HasCheckedIsSupported;
        static bool s_IsSupported;

        StringMap GetProcessEnvironmentVariables() const;
        StringMap GetUserOrMachineEnvironmentVariables() const;

        std::wstring GetProcessEnvironmentVariable(const std::wstring variableName) const;
        std::wstring GetUserOrMachineEnvironmentVariable(const std::wstring variableName) const;

        wil::unique_hkey GetRegHKeyForEVUserAndMachineScope(bool needsWriteAccess = false) const;

        std::wstring GetPath() const;
        std::wstring GetPathExt() const;

        void DeleteEnvironmentVariableIfExists(const HKEY hkey, const std::wstring name) const;
    };
}
namespace winrt::Microsoft::Windows::System::factory_implementation
{
    struct EnvironmentManager : EnvironmentManagerT<EnvironmentManager, implementation::EnvironmentManager>
    {
    };
}
