﻿#include "pch.h"
#include <testdef.h>
#include <iostream>
#include <sstream>
#include <wil/win32_helpers.h>
#include <winrt/Windows.ApplicationModel.Background.h> // we need this for BackgroundTask APIs
#include <winrt/Windows.Data.Json.h>
#include <winrt/Windows.Web.Http.h>
#include "WindowsAppRuntime.Test.AppModel.h"

using namespace winrt;
using namespace winrt::Microsoft::Windows::AppLifecycle;
using namespace winrt::Microsoft::Windows::PushNotifications;
using namespace winrt::Windows::ApplicationModel::Activation;
using namespace winrt::Windows::ApplicationModel::Background; // BackgroundTask APIs
using namespace winrt::Windows::Data::Json;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Storage;
using namespace winrt::Windows::Storage::Streams;
using namespace winrt::Windows::Web::Http;

winrt::guid remoteId1(L"cea1342d-8293-4acb-b18a-ed8b0d6f7d6c"); // Generated from ms.portal.azure.com
winrt::guid remoteId2(L"CA1A4AB2-AC1D-4EFC-A132-E5A191CA285A"); // Dummy guid from visual studio guid tool generator

constexpr auto timeout{ std::chrono::seconds(300) };

// Keep this in sync. Payload in both formats because hstring only accepts Wide string
// but we are serializing it across the wire as UTF8 and receiving it back in the same format.
std::wstring g_wideStringTestPayload = L"WindowsAppSDKTestPayload";
std::string g_narrowStringTestPayload = "WindowsAppSDKTestPayload";

wil::unique_event g_pushNotificationReceived;

bool ChannelRequestUsingNullRemoteId()
{
    try
    {
        auto channelOperation = PushNotificationManager::Default().CreateChannelAsync(winrt::guid()).get();
    }
    catch (...)
    {
        return to_hresult() == E_INVALIDARG;
    }
    return false;
}

HRESULT ChannelRequestHelper(IAsyncOperationWithProgress<PushNotificationCreateChannelResult, PushNotificationCreateChannelStatus> const& channelOperation)
{
    if (channelOperation.wait_for(timeout) != AsyncStatus::Completed)
    {
        channelOperation.Cancel();
        return HRESULT_FROM_WIN32(ERROR_TIMEOUT); // timed out or failed
    }

    auto result = channelOperation.GetResults();
    auto status = result.Status();
    if (status != PushNotificationChannelStatus::CompletedSuccess)
    {
        return result.ExtendedError(); // did not produce a channel
    }

    //result.Channel().Close();
    return S_OK;
}

HRESULT RequestPushNotificationFromServer(Uri channelUri, winrt::hstring payloadAsString)
{
    HttpClient httpClient;
    Uri serverUri(winrt::to_hstring("http://localhost:7071/api/PostPushNotificationforChannel"));

    JsonObject postNotificationRequest;
    postNotificationRequest.SetNamedValue(winrt::hstring(L"ChannelUri"), JsonValue::CreateStringValue(channelUri.ToString()));
    postNotificationRequest.SetNamedValue(winrt::hstring(L"X_WNS_Type"), JsonValue::CreateStringValue(winrt::hstring(L"wns/raw")));
    postNotificationRequest.SetNamedValue(winrt::hstring(L"Content_Type"), JsonValue::CreateStringValue(winrt::hstring(L"application/octet-stream")));
    postNotificationRequest.SetNamedValue(winrt::hstring(L"Payload"), JsonValue::CreateStringValue(payloadAsString));

    IHttpContent content = HttpStringContent(postNotificationRequest.ToString(), UnicodeEncoding::Utf8, winrt::hstring(L"application/json"));
    HttpRequestMessage httpRequestMessage;
    httpRequestMessage.RequestUri(serverUri);
    httpRequestMessage.Method(HttpMethod::Post());
    httpRequestMessage.Content(content);

    auto request = httpClient.SendRequestAsync(httpRequestMessage);
    if (request.wait_for(std::chrono::seconds(5)) != AsyncStatus::Completed)
    {
        request.Cancel();
        return HRESULT_FROM_WIN32(ERROR_TIMEOUT); // timed out or failed
    }

    request.GetResults().EnsureSuccessStatusCode();

    auto readOperation = request.GetResults().Content().ReadAsStringAsync();
    if (readOperation.wait_for(std::chrono::seconds(5)) != AsyncStatus::Completed)
    {
        readOperation.Cancel();
        return HRESULT_FROM_WIN32(ERROR_TIMEOUT); // timed out or failed
    }

    std::wcout << readOperation.GetResults().c_str() << std::endl;
}

bool ChannelRequestUsingRemoteId()
{
    auto channelOperation = PushNotificationManager::Default().CreateChannelAsync(remoteId1);
    auto channelOperationResult = ChannelRequestHelper(channelOperation);

    return channelOperationResult == S_OK;
}

// Verify calling channel close will fail when called twice.
bool MultipleChannelClose()
{
    auto channelOperation = PushNotificationManager::Default().CreateChannelAsync(remoteId1);
    if (channelOperation.wait_for(timeout) != AsyncStatus::Completed)
    {
        channelOperation.Cancel();
        return false; // timed out or failed
    }

    auto result = channelOperation.GetResults();
    auto status = result.Status();
    if (status != PushNotificationChannelStatus::CompletedSuccess)
    {
        return false; // did not produce a channel
    }

    result.Channel().Close();
    try
    {
        result.Channel().Close();
    }
    catch (...)
    {
        return to_hresult() == WPN_E_CHANNEL_CLOSED;
    }
    return false;
}

bool BackgroundActivationTest() // Activating application for background test.
{
    PushNotificationManager::Default().UnregisterAll();
    return true;
}

bool VerifyRegisterandUnregisterAll()
{
    try
    {
        // Register is already called in main
        PushNotificationManager::Default().UnregisterAll();
    }
    catch (...)
    {
        return false;
    }
    return true;
}

bool VerifyRegisterandUnregister()
{
    try
    {
        // Register is already called in main
        PushNotificationManager::Default().Unregister();
    }
    catch (...)
    {
        return false;
    }
    return true;
}

// Verify registering multiple activators is not allowed.
bool MultipleRegister()
{
    try
    {
        PushNotificationManager::Default().Register();
    }
    catch (...)
    {
        return to_hresult() == HRESULT_FROM_WIN32(ERROR_ALREADY_REGISTERED);
    }
    return false;
}

bool VerifyMultipleRegisterAndUnregister()
{
    try
    {
        // Register is already called in main
        PushNotificationManager::Default().Unregister();

        PushNotificationManager::Default().Register();

        PushNotificationManager::Default().Unregister();
    }
    catch (...)
    {
        return to_hresult() == HRESULT_FROM_WIN32(ERROR_ALREADY_REGISTERED);
    }
    return false;
}

bool VerifyMultipleRegisterAndUnregisterAll()
{
    try
    {
        // Register is already called in main
        PushNotificationManager::Default().UnregisterAll();

        PushNotificationManager::Default().Register();

        PushNotificationManager::Default().UnregisterAll();
    }
    catch (...)
    {
        return false;
    }
    return true;
}

bool VerifyUnregisterAndUnregisterAll()
{
    try
    {
        // Register is already called in main
        PushNotificationManager::Default().Unregister();
        PushNotificationManager::Default().UnregisterAll();
    }
    catch (...)
    {
        return false;
    }
    return true;
}

bool VerifyUnregisterTwice()
{
    try
    {
        // Register is already called in main
        PushNotificationManager::Default().Unregister();
        PushNotificationManager::Default().Unregister();
    }
    catch (...)
    {
        return winrt::to_hresult() == E_UNEXPECTED;
    }
    return false;
}

bool VerifyUnregisterAll()
{
    try
    {
        // Register is already called in main
        PushNotificationManager::Default().UnregisterAll();
    }
    catch (...)
    {
        return false;
    }
    return true;
}

bool VerifyUnregisterAllTwice()
{
    try
    {
        // Register is already called in main
        PushNotificationManager::Default().UnregisterAll();
        PushNotificationManager::Default().UnregisterAll();
    }
    catch (...)
    {
        return winrt::to_hresult() == E_UNEXPECTED;
    }
    return true;
}

bool VerifyForegroundHandlerSucceeds()
{
    PushNotificationManager::Default().UnregisterAll();
    try
    {
        PushNotificationManager::Default().PushReceived([](const auto&, PushNotificationReceivedEventArgs const& args) {});
        PushNotificationManager::Default().Register();
    }
    catch (...)
    {
        return false;
    }
    return true;
}

bool VerifyForegroundHandlerFails()
{
    try
    {
        // Register is already called in main
        PushNotificationManager::Default().PushReceived([](const auto&, PushNotificationReceivedEventArgs const& args) {});
    }
    catch (...)
    {
        return true;
    }
    return false;
}

bool VerifyPushReceivedInForeground()
{
    PushNotificationManager::Default().UnregisterAll();
    try
    {
        g_pushNotificationReceived.create();
        PushNotificationManager::Default().PushReceived([](const auto&, PushNotificationReceivedEventArgs const& args)
            {
                std::string payloadAsString = std::string(reinterpret_cast<char*>(args.Payload().data()), args.Payload().size());
                std::cout << g_narrowStringTestPayload << std::endl;
                std::cout << "Payload size is: " << args.Payload().size() << std::endl;
                std::cout << payloadAsString << std::endl;

                if (g_narrowStringTestPayload == payloadAsString)
                {
                    g_pushNotificationReceived.SetEvent();
                }
            });
        PushNotificationManager::Default().Register();
        auto channelOperation = PushNotificationManager::Default().CreateChannelAsync(remoteId1);
        check_hresult(ChannelRequestHelper(channelOperation));
        std::wcout << channelOperation.GetResults().Channel().Uri().ToString().c_str() << std::endl;
        RequestPushNotificationFromServer(channelOperation.GetResults().Channel().Uri(), winrt::hstring(g_wideStringTestPayload));

        bool payloadReceived = false;
        if (g_pushNotificationReceived.wait(30000))
        {
            payloadReceived = true;
        }

        channelOperation.GetResults().Channel().Close();
        return payloadReceived;
    }
    catch (...)
    {
        return false;
    }

    return true;
}


std::map<std::string, bool(*)()> const& GetSwitchMapping()
{
    static std::map<std::string, bool(*)()> switchMapping = {
        { "ChannelRequestUsingNullRemoteId",  &ChannelRequestUsingNullRemoteId },
        { "ChannelRequestUsingRemoteId", &ChannelRequestUsingRemoteId },
        { "MultipleChannelClose", &MultipleChannelClose},
        
        { "BackgroundActivationTest", &BackgroundActivationTest},

        { "VerifyRegisterandUnregister", &VerifyRegisterandUnregister},
        { "VerifyRegisterandUnregisterAll", &VerifyRegisterandUnregisterAll},
        { "MultipleRegister", &MultipleRegister},
        { "VerifyMultipleRegisterAndUnregister", &VerifyMultipleRegisterAndUnregister},
        { "VerifyMultipleRegisterAndUnregisterAll", &VerifyMultipleRegisterAndUnregisterAll},

        { "VerifyUnregisterAndUnregisterAll", &VerifyUnregisterAndUnregisterAll},
        { "VerifyUnregisterTwice", &VerifyUnregisterTwice},
        { "VerifyUnregisterAll", &VerifyUnregisterAll},
        { "VerifyUnregisterAllTwice", &VerifyUnregisterAllTwice},

        { "VerifyForegroundHandlerSucceeds", &VerifyForegroundHandlerSucceeds },
        { "VerifyForegroundHandlerFails", &VerifyForegroundHandlerFails },

        { "VerifyPushReceivedInForeground", &VerifyPushReceivedInForeground }
    };
    return switchMapping;
}

bool runUnitTest(std::string unitTest)
{
    auto const& switchMapping = GetSwitchMapping();
    auto it = switchMapping.find(unitTest);
    if (it == switchMapping.end())
    {
        return false;
    }

    return it->second();
}

std::string unitTestNameFromLaunchArguments(const ILaunchActivatedEventArgs& launchArgs)
{
    std::string unitTestName = to_string(launchArgs.Arguments());
    auto argStart = unitTestName.rfind(" ");
    if (argStart != std::wstring::npos)
    {
        unitTestName = unitTestName.substr(argStart + 1);
    }

    return unitTestName;
}

int main() try
{
    bool testResult = false;
    auto scope_exit = wil::scope_exit([&] {  
        ::Test::Bootstrap::CleanupBootstrap();
    });

    ::Test::Bootstrap::SetupBootstrap();

    PushNotificationManager::Default().Register();
    
    auto args = AppInstance::GetCurrent().GetActivatedEventArgs();
    auto kind = args.Kind();

    if (kind == ExtendedActivationKind::Launch)
    {
        auto unitTest = unitTestNameFromLaunchArguments(args.Data().as<ILaunchActivatedEventArgs>());
        std::cout << unitTest << std::endl;

        testResult = runUnitTest(unitTest);
    }
    else if (kind == ExtendedActivationKind::Push)
    {
        PushNotificationReceivedEventArgs pushArgs = args.Data().as<PushNotificationReceivedEventArgs>();
        auto payload = pushArgs.Payload();
        std::wstring payloadString(payload.begin(), payload.end());

        testResult = payloadString == c_rawNotificationPayload;
    }

    return testResult ? 0 : 1; // We want 0 to be success and 1 failure
}
catch (...)
{
    std::cout << winrt::to_string(winrt::to_message()) << std::endl;
    return 1; // in the event of unhandled test crash
}
