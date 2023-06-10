﻿// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#pragma once

#include "pch.h"
#include "InstallActivityContext.h"

using namespace WindowsAppRuntimeInstaller::InstallActivity;

#define InstallActivity_ExceptionFailFast_StopWithResult() \
installActivityContext.GetActivity().StopWithResult( \
    failure.hr,\
    static_cast<UINT32>(failure.type),\
    failure.pszFile,\
    failure.uLineNumber,\
    failure.pszMessage,\
    static_cast<UINT32>(installActivityContext.GetInstallStage()),\
    installActivityContext.GetCurrentResourceId().c_str(),\
    installActivityContext.GetDeploymentErrorExtendedHResult(),\
    installActivityContext.GetDeploymentErrorText().c_str(),\
    installActivityContext.GetDeploymentErrorActivityId())

// A process-wide callback function for WIL Error Handlers
void __stdcall wilResultLoggingCallback(const wil::FailureInfo& failure) noexcept
{
    if (WindowsAppRuntimeInstaller_TraceLogger::IsEnabled())
    {
        auto& installActivityContext{ WindowsAppRuntimeInstaller::InstallActivity::Context::Get() };

        if (installActivityContext.GetActivity().IsRunning())
        {
            switch (failure.type)
            {
            case wil::FailureType::Log:
            {
                // wil Log failure type indicates intention to just log failure but continue with the installation

                if (installActivityContext.GetInstallStage() == InstallStage::ProvisionPackage)
                {
                    // Failure in Provisioning package are non-blocking and the installer will continue with installation
                    WindowsAppRuntimeInstaller_WriteEventWithActivity(
                        "ProvisioningFailed",
                        WindowsAppRuntimeInstaller_TraceLoggingWString(installActivityContext.GetCurrentResourceId(), "currentPackage"),
                        _GENERIC_PARTB_FIELDS_ENABLED,
                        TelemetryPrivacyDataTag(PDT_ProductAndServicePerformance),
                        TraceLoggingKeyword(MICROSOFT_KEYWORD_CRITICAL_DATA));
                }
                else if (installActivityContext.GetInstallStage() == InstallStage::RestartPushNotificationsLRP)
                {
                    // Failure in restarting PushNotificationsLRP is non-blocking to the installer functionality
                    WindowsAppRuntimeInstaller_WriteEventWithActivity(
                        "RestartPushNotificationsLRPFailed",
                        _GENERIC_PARTB_FIELDS_ENABLED,
                        TelemetryPrivacyDataTag(PDT_ProductAndServicePerformance),
                        TraceLoggingKeyword(MICROSOFT_KEYWORD_CRITICAL_DATA));
                }
                else
                {
                    WindowsAppRuntimeInstaller_WriteEventWithActivity("FailureLog");
                }
                break;
            }
            case wil::FailureType::Exception:
            {
                WindowsAppRuntimeInstaller_WriteEventWithActivity(
                    "Exception",
                    TraceLoggingCountedWideString(
                        installActivityContext.GetCurrentResourceId().c_str(),
                        static_cast<ULONG>(installActivityContext.GetCurrentResourceId().size()), "currentResource"));

                InstallActivity_ExceptionFailFast_StopWithResult();

                break;
            }
            case wil::FailureType::FailFast:
            {
                WindowsAppRuntimeInstaller_WriteEventWithActivity(
                    "FailFast",
                    TraceLoggingCountedWideString(
                        installActivityContext.GetCurrentResourceId().c_str(),
                        static_cast<ULONG>(installActivityContext.GetCurrentResourceId().size()), "currentResource"));

                InstallActivity_ExceptionFailFast_StopWithResult();

                break;
            }
            case wil::FailureType::Return:
            {
                // THROW_*** error handling combined with CATCH_RETURN in the code may log the same failure twice.
                // That's ok and we can live with that redundancy but don't want to lose failure info from RETURN_*** wil macros.
                WindowsAppRuntimeInstaller_WriteEventWithActivity("FailureReturn");

                // Don't stop the Install activity here. Instead, give the Installer main a chance to Stop the Activity before returning error to the caller.
                // Hence, save the wil failure info here for later use.
                installActivityContext.SetLastFailure(failure);
                break;
            }
            default:
                break;
            }
        }
        return;
    }
}
