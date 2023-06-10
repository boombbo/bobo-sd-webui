﻿// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.
#include "pch.h"
#include <testdef.h>
#include <wil/win32_helpers.h>
#include <iostream>
#include <time.h>
#include <winrt/Windows.ApplicationModel.Background.h> // we need this for BackgroundTask APIs

#define SIZE 26

using namespace winrt::Microsoft::Windows::AppLifecycle;
using namespace winrt::Microsoft::Windows::PushNotifications;
using namespace winrt;
using namespace winrt::Windows::Storage;
using namespace winrt::Windows::Storage::Streams;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::ApplicationModel::Activation;
using namespace winrt::Windows::ApplicationModel::Background; // BackgroundTask APIs

time_t ltime;
char buf[SIZE];

void RequestChannel()
{
    // Register the AAD RemoteIdentifier for the App to receive Push
    auto channelOperation = PushNotificationManager::CreateChannelAsync(
        winrt::guid("ccd2ae3f-764f-4ae3-be45-9804761b28b2"));

    // Setup the inprogress event handler
    channelOperation.Progress(
        [](
            IAsyncOperationWithProgress<PushNotificationCreateChannelResult, PushNotificationCreateChannelStatus> const& sender,
            PushNotificationCreateChannelStatus const& args)
        {
            if (args.status == PushNotificationChannelStatus::InProgress)
            {
                // This is basically a noop since it isn't really an error state
                std::cout << "Channel request is in progress." << std::endl << std::endl;
            }
            else if (args.status == PushNotificationChannelStatus::InProgressRetry)
            {
                LOG_HR_MSG(
                    args.extendedError,
                    "The channel request is in back-off retry mode because of a retryable error! Expect delays in acquiring it. RetryCount = %d",
                    args.retryCount);
            }
        });

    winrt::event_token pushToken;

    // Setup the completed event handler
    channelOperation.Completed(
        [&](
            IAsyncOperationWithProgress<PushNotificationCreateChannelResult, PushNotificationCreateChannelStatus> const& sender,
            AsyncStatus const asyncStatus)
        {
            auto result = sender.GetResults();
            if (result.Status() == PushNotificationChannelStatus::CompletedSuccess)
            {
                auto channelUri = result.Channel().Uri();

                std::cout << "channelUri: " << winrt::to_string(channelUri.ToString()) << std::endl << std::endl;

                auto channelExpiry = result.Channel().ExpirationTime();

                // Register Push Event for Foreground
                pushToken = result.Channel().PushReceived([&](const auto&, PushNotificationReceivedEventArgs args)
                    {
                        auto payload = args.Payload();

                        // Do stuff to process the raw payload
                        std::string payloadString(payload.begin(), payload.end());
                        std::cout << "Push notification content received from FOREGROUND: " << payloadString << std::endl << std::endl;
                        args.Handled(true);
                    });
            }
            else if (result.Status() == PushNotificationChannelStatus::CompletedFailure)
            {
                LOG_HR_MSG(result.ExtendedError(), "We hit a critical non-retryable error with channel request!");
            }
        });
}

int main()
{
    time(&ltime);
    ctime_s(buf, sizeof buf, &ltime);
    std::cout << "Project Reunion Push Notification Test App: " << buf << std::endl;

    PushNotificationActivationInfo info(
        PushNotificationRegistrationKind::PushTrigger | PushNotificationRegistrationKind::ComActivator,
        winrt::guid("ccd2ae3f-764f-4ae3-be45-9804761b28b2")); // same clsid as app manifest

    auto token = PushNotificationManager::RegisterActivator(info);

    auto args = AppInstance::GetCurrent().GetActivatedEventArgs();
    auto kind = args.Kind();
    if (kind == ExtendedActivationKind::Push)
    {
        PushNotificationReceivedEventArgs pushArgs = args.Data().as<PushNotificationReceivedEventArgs>();

        // Call GetDeferral to ensure that code runs in low power
        auto deferral = pushArgs.GetDeferral();

        auto payload = pushArgs.Payload();

        // Do stuff to process the raw payload
        std::string payloadString(payload.begin(), payload.end());
        std::cout << "Push notification content received from BACKGROUND: " << payloadString << std::endl << std::endl;
        std::cout << "Press 'Enter' to exit the App." << std::endl;
        std::cin.ignore();
        // Call Complete on the deferral as good practise: Needed mainly for low power usage
        deferral.Complete();
    }
    else if (kind == ExtendedActivationKind::Launch)
    {
        RequestChannel();
        std::cout << "Press 'Enter' at any time to exit App." << std::endl << std::endl;
        std::cin.ignore();
    }
    else if (kind == ExtendedActivationKind::ToastNotification)
    {
        std::cout << "ToastNotification received!" << std::endl << std::endl;
        std::cout << "Press 'Enter' at any time to exit App." << std::endl << std::endl;
        std::cin.ignore();
    }

    PushNotificationManager::UnregisterActivator(token, PushNotificationRegistrationKind::ComActivator);
    return 0;
}
