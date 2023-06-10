﻿// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"

// include notifications constants file here

#include <NotificationsReunionEndpoint_h.h>
#include <EventToken.h>
// Temporarily disable C4324 because WRL generates a false (well, irrelevant) warning
//   'Microsoft::WRL::Details::StaticStorage<Microsoft::WRL::Details::OutOfProcModuleBase<ModuleT>::GenericReleaseNotifier<T>,Microsoft::WRL::Details::StorageInstance::OutOfProcCallbackBuffer1,ModuleT>': structure was padded due to alignment specifier
#pragma warning(push)
#pragma warning(disable:4324)
#include <wrl.h>
#pragma warning(pop)
#include <winrt/Windows.Networking.PushNotifications.h>
#include <winrt/Windows.Foundation.h>
#include <Windows.Foundation.h>

using namespace Microsoft::WRL;
using namespace winrt::Windows::Networking::PushNotifications;
wil::unique_event g_endOfTheLine;

void EndOfTheLine()
{
    g_endOfTheLine.SetEvent();
}

struct __declspec(uuid("A15FBAC0-8D22-472A-A036-78A8BE334FDE")) WpnForegroundSinkImpl WrlFinal : RuntimeClass<RuntimeClassFlags<ClassicCom>, IWpnForegroundSink>
{
    typedef winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Networking::PushNotifications::PushNotificationChannel*, winrt::Windows::Networking::PushNotifications::PushNotificationReceivedEventArgs*> IPushNotificationReceivedEventHandler;

private:
        Microsoft::WRL::AgileEventSource<IUnknown> m_event; // watch for RPC errors
        mutable Microsoft::WRL::Wrappers::SRWLock m_lock;
        Microsoft::WRL::ComPtr<EventRegistrationToken> token{};
        //EventRegistrationToken token;
    public:
        STDMETHODIMP Invoke()
        {
            auto lock = m_lock.LockExclusive();
            m_event.InvokeAll();
            return S_OK;
        }

        STDMETHODIMP AddEvent(/*[in]*/ IPushNotificationReceivedEventHandler* handler)
        {
            auto lock = m_lock.LockExclusive();
            return m_event.Add(handler->as<IUnknown*>(), token.Get());
        }

        STDMETHODIMP RemoveEvent()
        {
            auto lock = m_lock.LockExclusive();
            return m_event.Remove(*token.Get());
        }
};
CoCreatableClass(WpnForegroundSinkImpl);

struct __declspec(uuid("330EC755-31F2-40A7-977D-B0ABB1E1E52E")) NotificationsReunionEndpointImpl WrlFinal : RuntimeClass<RuntimeClassFlags<ClassicCom>, INotificationsReunionEndpoint>
{
    STDMETHODIMP GetStringFromLRP(/*[out, retval]*/ LPWSTR* packageFullName)
    {
        *packageFullName = nullptr;

        // working too hard I guess
        size_t length = 0;
        StringCchLength(L"Hi from the Long Running Process UwU", STRSAFE_MAX_CCH, &length);

        PWSTR copy = reinterpret_cast<PWSTR>(CoTaskMemAlloc(sizeof(WCHAR) * (length + 1)));
        RETURN_IF_NULL_ALLOC(copy);

        RETURN_IF_FAILED(StringCchCopy(copy, length + 1, L"Hi from the Long Running Process UwU"));

        *packageFullName = copy;
        return S_OK;
    }

    STDMETHODIMP GetStringLength(/*[in]*/ PCWSTR string, /*[out]*/ ULONG* length)
    {
        // working too hard
        size_t theLength = wcslen(string);
        *length = theLength;
        return S_OK;
    }
};
CoCreatableClass(NotificationsReunionEndpointImpl);

int WINAPI WinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, PSTR lpCmdLine, int nCmdShow)
{
    //Sleep(20000);

    // if (lpCmdLine contains SomeCmdArg)
    //     handle COM activation
    // else
    //     handle Startup activation
    RETURN_IF_FAILED(::CoInitializeEx(nullptr, COINITBASE_MULTITHREADED));

    wil::unique_event endOfTheLine(::CreateEventW(nullptr, TRUE, FALSE, nullptr));
    RETURN_LAST_ERROR_IF_NULL(endOfTheLine);
    g_endOfTheLine = std::move(endOfTheLine);

    auto& module = Module<OutOfProc>::Create(EndOfTheLine); // Function to signal event

    module.IncrementObjectCount();

    RETURN_IF_FAILED(module.RegisterObjects());

    g_endOfTheLine.wait();

    (void)LOG_IF_FAILED(module.UnregisterObjects());
    module.Terminate();

    ::CoUninitialize();

    return 0;
}

STDAPI_(BOOL) DllMain(_In_opt_ HINSTANCE hinst, DWORD reason, _In_opt_ void*)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hinst);
    }
    return TRUE;
}
