﻿#include "pch.h"
#include <testdef.h>
#include <iostream>
#include <sstream>
#include <regex>

using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace winrt::Microsoft::Windows::PushNotifications;
using namespace winrt::Windows::ApplicationModel::Activation;

enum UnitTest {
    channelRequestUsingNullRemoteId, channelRequestUsingRemoteId, multipleChannelRequestUsingSameRemoteId,
    multipleChannelRequestUsingMultipleRemoteId, threeChannelRequestUsingSameRemoteId
};

static std::map<std::string, UnitTest> switchMapping;
winrt::guid remoteId1(L"a2e4a323-b518-4799-9e80-0b37aeb0d225");
winrt::guid remoteId2(L"CA1A4AB2-AC1D-4EFC-A132-E5A191CA285A");
winrt::guid remoteId3(L"40FCE789-C6BF-4F47-A6CF-6B9C1DCE31BA");

void signalPhase(const std::wstring& phaseEventName)
{
    wil::unique_event phaseEvent;
    if (phaseEvent.try_open(phaseEventName.c_str(), EVENT_MODIFY_STATE, false))
    {
        phaseEvent.SetEvent();
    }
}

void initUnitTestMapping()
{
    switchMapping["ChannelRequestUsingNullRemoteId"] = UnitTest::channelRequestUsingNullRemoteId;
    switchMapping["ChannelRequestUsingRemoteId"] = UnitTest::channelRequestUsingRemoteId;
    switchMapping["MultipleChannelRequestUsingSameRemoteId"] = UnitTest::multipleChannelRequestUsingSameRemoteId;
    switchMapping["MultipleChannelRequestUsingMultipleRemoteId"] = UnitTest::multipleChannelRequestUsingMultipleRemoteId;
    switchMapping["ThreeChannelRequestUsingSameRemoteId"] = UnitTest::threeChannelRequestUsingSameRemoteId;
}

bool ChannelRequestUsingNullRemoteId()
{
    winrt::hresult hr = S_OK;

    try
    {
        auto channelOperation = PushNotificationManager::CreateChannelAsync(GUID_NULL).get();
    }
    catch (...)
    {
        auto channelRequestException = hresult_error(to_hresult());
        hr = channelRequestException.code();
    }

    return (hr == E_INVALIDARG);
}

bool ChannelRequestUsingRemoteId()
{
    wil::unique_handle channelEvent = wil::unique_handle(CreateEvent(nullptr, FALSE, FALSE, nullptr));
    auto hr = S_OK;
    auto channelOperation = PushNotificationManager::CreateChannelAsync(remoteId1);

    channelOperation.Progress(
        [](
            IAsyncOperationWithProgress<PushNotificationCreateChannelResult, PushNotificationCreateChannelStatus> const& /* sender */,
            PushNotificationCreateChannelStatus const& args)
        {
            if (args.status == PushNotificationChannelStatus::InProgress)
            {
            }
            else if (args.status == PushNotificationChannelStatus::InProgressRetry)
            {
            }
        });

    // Setup the completed event handler
    channelOperation.Completed(
        [&channelEvent](
            IAsyncOperationWithProgress<PushNotificationCreateChannelResult, PushNotificationCreateChannelStatus> const& sender,
            AsyncStatus const /* asyncStatus */)
        {
            auto result = sender.GetResults();
            if (result.Status() == PushNotificationChannelStatus::CompletedSuccess)
            {
                auto channelUri = result.Channel().Uri();
                auto channelExpiry = result.Channel().ExpirationTime();
            }
            else if (result.Status() == PushNotificationChannelStatus::CompletedFailure)
            {
            }

            SetEvent(channelEvent.get());
        });

    // The maximum amount of time it takes for channel request to be obtained - 16mins
    if (WAIT_OBJECT_0 != WaitForSingleObject(channelEvent.get(), 960000 /* milliseconds */))
    {
        channelOperation.Cancel();
        hr = ERROR_TIMEOUT;
    }
    else
    {
        channelOperation.Close(); // Do not call getresults after this
    }

    return (hr == S_OK);
}

bool MultipleChannelRequestUsingSameRemoteId()
{
    wil::unique_handle channelEvent = wil::unique_handle(CreateEvent(nullptr, FALSE, FALSE, nullptr));
    auto hr = S_OK;

    auto channelOperation1 = PushNotificationManager::CreateChannelAsync(remoteId1);
    auto channelOperation2 = PushNotificationManager::CreateChannelAsync(remoteId1);

    channelOperation2.Completed(
        [&channelEvent, &hr](
            IAsyncOperationWithProgress<PushNotificationCreateChannelResult, PushNotificationCreateChannelStatus> const& sender,
            AsyncStatus const /* asyncStatus */)
        {
            auto result = sender.GetResults();
            if (result.Status() == PushNotificationChannelStatus::CompletedSuccess)
            {
                auto channelUri = result.Channel().Uri();
                auto channelExpiry = result.Channel().ExpirationTime();

            }
            else if (result.Status() == PushNotificationChannelStatus::CompletedFailure)
            {
                hr = result.ExtendedError();
            }


            SetEvent(channelEvent.get());
        });

    // The maximum amount of time it takes for channel request to be obtained - 16mins
    if (WAIT_OBJECT_0 != WaitForSingleObject(channelEvent.get(), 960000 /* milliseconds */))
    {
        channelOperation2.Cancel();
        hr = ERROR_TIMEOUT;
    }

    return (hr == WPN_E_OUTSTANDING_CHANNEL_REQUEST);
}

bool MultipleChannelRequestUsingMultipleRemoteId()
{
    wil::unique_handle channelEvent = wil::unique_handle(CreateEvent(nullptr, FALSE, FALSE, nullptr));
    auto channelOperationResult2 = S_OK;

    auto channelOperation1 = PushNotificationManager::CreateChannelAsync(remoteId1);
    auto channelOperation2 = PushNotificationManager::CreateChannelAsync(remoteId2);

    channelOperation2.Completed(
        [&channelEvent, &channelOperationResult2](
            IAsyncOperationWithProgress<PushNotificationCreateChannelResult, PushNotificationCreateChannelStatus> const& sender,
            AsyncStatus const /* asyncStatus */)
        {
            auto result = sender.GetResults();
            if (result.Status() == PushNotificationChannelStatus::CompletedSuccess)
            {
                auto channelUri = result.Channel().Uri();
                auto channelExpiry = result.Channel().ExpirationTime();
            }
            else if (result.Status() == PushNotificationChannelStatus::CompletedFailure)
            {
                channelOperationResult2 = result.ExtendedError();
            }

            SetEvent(channelEvent.get());
        });

    // The maximum amount of time it takes for channel request to be obtained - 16mins
    if (WAIT_OBJECT_0 != WaitForSingleObject(channelEvent.get(), 960000 /* milliseconds */))
    {
        channelOperation2.Cancel();
        channelOperationResult2 = ERROR_TIMEOUT;
    }

    return (channelOperationResult2 == WPN_E_OUTSTANDING_CHANNEL_REQUEST);
}

bool ThreeChannelRequestUsingSameRemoteId()
{
    wil::unique_handle channelEvent2 = wil::unique_handle(CreateEvent(nullptr, FALSE, FALSE, nullptr));
    auto channelOperationResult2 = S_OK;

    wil::unique_handle channelEvent3 = wil::unique_handle(CreateEvent(nullptr, FALSE, FALSE, nullptr));
    auto channelOperationResult3 = S_OK;

    auto channelOperation1 = PushNotificationManager::CreateChannelAsync(remoteId1);
    auto channelOperation2 = PushNotificationManager::CreateChannelAsync(remoteId2);

    channelOperation2.Completed(
        [&channelEvent2, &channelOperationResult2](
            IAsyncOperationWithProgress<PushNotificationCreateChannelResult, PushNotificationCreateChannelStatus> const& sender,
            AsyncStatus const /* asyncStatus */)
        {
            auto result = sender.GetResults();
            if (result.Status() == PushNotificationChannelStatus::CompletedSuccess)
            {
                auto channelUri = result.Channel().Uri();
                auto channelExpiry = result.Channel().ExpirationTime();
            }
            else if (result.Status() == PushNotificationChannelStatus::CompletedFailure)
            {
                channelOperationResult2 = result.ExtendedError();
            }

            SetEvent(channelEvent2.get());
        });


    auto channelOperation3 = PushNotificationManager::CreateChannelAsync(remoteId3);

    channelOperation3.Completed(
        [&channelEvent3, &channelOperationResult3](
            IAsyncOperationWithProgress<PushNotificationCreateChannelResult, PushNotificationCreateChannelStatus> const& sender,
            AsyncStatus const /* asyncStatus */)
        {
            auto result = sender.GetResults();
            if (result.Status() == PushNotificationChannelStatus::CompletedSuccess)
            {
                auto channelUri = result.Channel().Uri();
                auto channelExpiry = result.Channel().ExpirationTime();
            }
            else if (result.Status() == PushNotificationChannelStatus::CompletedFailure)
            {
                channelOperationResult3 = result.ExtendedError();
            }

            SetEvent(channelEvent3.get());
        });

    // The maximum amount of time it takes for channel request to be obtained - 16mins
    if (WAIT_OBJECT_0 != WaitForSingleObject(channelEvent2.get(), 960000 /* milliseconds */))
    {
        channelOperation2.Cancel();
        channelOperationResult2 = ERROR_TIMEOUT;
    }

    // The maximum amount of time it takes for channel request to be obtained - 16mins
    if (WAIT_OBJECT_0 != WaitForSingleObject(channelEvent3.get(), 960000 /* milliseconds */))
    {
        channelOperation3.Cancel();
        channelOperationResult3 = ERROR_TIMEOUT;
    }

    return ((channelOperationResult2 == WPN_E_OUTSTANDING_CHANNEL_REQUEST) && (channelOperationResult3 == WPN_E_OUTSTANDING_CHANNEL_REQUEST));
}

bool runUnitTest(std::string unitTest)
{
    switch (switchMapping[unitTest])
    {
    case UnitTest::channelRequestUsingNullRemoteId:
        return ChannelRequestUsingNullRemoteId();

    case UnitTest::channelRequestUsingRemoteId:
        return ChannelRequestUsingRemoteId();

    case UnitTest::multipleChannelRequestUsingSameRemoteId:
        return MultipleChannelRequestUsingSameRemoteId();

    case UnitTest::multipleChannelRequestUsingMultipleRemoteId:
        return MultipleChannelRequestUsingMultipleRemoteId();

    case UnitTest::threeChannelRequestUsingSameRemoteId:
        return ThreeChannelRequestUsingSameRemoteId();

    default:
        return false;
    }
}

int main()
{
    initUnitTestMapping();
    bool succeeded = true;
    auto args = winrt::Microsoft::ApplicationModel::Activation::AppLifecycle::GetActivatedEventArgs(); // mock name for now
    auto kind = args.Kind();

    if (kind == ActivationKind::Protocol)
    {
        auto protocolArgs = args.as<IProtocolActivatedEventArgs>();
        Uri actualUri = protocolArgs.Uri();
        std::string unitTest = winrt::to_string(actualUri.Host());

        std::cout << unitTest << std::endl;

        // Switch on this variable to run specific components (uri://ComponentToTest)
        auto output = runUnitTest(unitTest);

        if (output)
        {
            // Signal TAEF that protocol was activated and valid.
            signalPhase(c_testProtocolScheme_Packaged);
        }
        else
        {
            // Signal TAEF that the test failed
            signalPhase(c_testFailureEventName);
        }
    }

    return 0;
};
