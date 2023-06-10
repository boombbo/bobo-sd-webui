﻿#pragma once

#include "../PushNotifications-Constants.h"

struct __declspec(uuid(PUSHNOTIFICATIONS_IMPL_CLSID_STRING)) NotificationsLongRunningPlatformImpl:
    winrt::implements<NotificationsLongRunningPlatformImpl, INotificationsLongRunningPlatform>
{
    void Initialize();

    void Shutdown() noexcept;

    void WaitForLifetimeEvent();

    /* INotificationsLongRunningPlatform functions */

    STDMETHOD(RegisterFullTrustApplication)(_In_ PCWSTR processName, GUID remoteId, _Out_ PWSTR* appId) noexcept;

    STDMETHOD(RegisterLongRunningActivator)(_In_ PCWSTR processName) noexcept;

    STDMETHOD(UnregisterLongRunningActivator)(_In_ PCWSTR processName) noexcept;

    STDMETHOD(RegisterForegroundActivator)(_In_ IWpnForegroundSink* sink, _In_ PCWSTR processName) noexcept;

    STDMETHOD(UnregisterForegroundActivator)(_In_ PCWSTR processName) noexcept;

    STDMETHOD(AddToastRegistration)(_In_ PCWSTR processName, _In_ PCWSTR toastGuid) noexcept;

    STDMETHOD(RemoveToastRegistration)(_In_ PCWSTR processName) noexcept;
private:

    std::map<std::wstring, std::wstring> GetFullTrustApps();
    const std::wstring GetAppIdentifier(std::wstring const& processName);
    void RemoveAppIdentifier(std::wstring const& processName);
    void RemoveToastRegistration(std::wstring const& processName);

    winrt::Windows::Storage::ApplicationDataContainer m_rawStorage{ nullptr };
    winrt::Windows::Storage::ApplicationDataContainer m_toastStorage{ nullptr };

    wil::srwlock m_lock;

    bool m_initialized = false;
    bool m_shutdown = false;

    PlatformLifetimeManager m_lifetimeManager{};
    NotificationListenerManager m_notificationListenerManager{};
    std::shared_ptr<ForegroundSinkManager> m_foregroundSinkManager;
    std::shared_ptr<ToastRegistrationManager> m_toastRegistrationManager;
};
