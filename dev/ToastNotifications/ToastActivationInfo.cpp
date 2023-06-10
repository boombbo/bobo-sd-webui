﻿#include "pch.h"
#include "ToastActivationInfo.h"
#include "Microsoft.Windows.ToastNotifications.ToastActivationInfo.g.cpp"

namespace winrt::Microsoft::Windows::ToastNotifications::implementation
{
    winrt::Microsoft::Windows::ToastNotifications::ToastActivationInfo ToastActivationInfo::CreateFromActivationGuid(winrt::guid const& taskClsid)
    {
        THROW_HR_IF_MSG(E_ILLEGAL_METHOD_CALL, !AppModel::Identity::IsPackagedProcess(), "Not applicable for unpackaged applications");

        return winrt::make<ToastActivationInfo>(taskClsid);
    }

    winrt::Microsoft::Windows::ToastNotifications::ToastActivationInfo ToastActivationInfo::CreateFromToastAssets(winrt::Microsoft::Windows::ToastNotifications::ToastAssets const& assets)
    {
        THROW_HR_IF_MSG(E_ILLEGAL_METHOD_CALL, AppModel::Identity::IsPackagedProcess(), "Not applicable for packaged applications");

        return winrt::make<ToastActivationInfo>(assets);
    }

    winrt::guid ToastActivationInfo::TaskClsid()
    {
        THROW_HR_IF_MSG(E_ILLEGAL_METHOD_CALL, !AppModel::Identity::IsPackagedProcess(), "Not applicable for unpackaged applications");

        return m_taskClsid;
    }

    winrt::Microsoft::Windows::ToastNotifications::ToastAssets ToastActivationInfo::Assets()
    {
        THROW_HR_IF_MSG(E_ILLEGAL_METHOD_CALL, AppModel::Identity::IsPackagedProcess(), "Not applicable for packaged applications");

        return m_assets;
    }
}
