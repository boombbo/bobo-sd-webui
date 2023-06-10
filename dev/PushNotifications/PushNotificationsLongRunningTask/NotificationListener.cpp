﻿#include "pch.h"

HRESULT NotificationListener::RuntimeClassInitialize(
    std::shared_ptr<ForegroundSinkManager> foregroundSinkManager,
    std::wstring appId,
    std::wstring processName)
{
    m_foregroundSinkManager = foregroundSinkManager;

    m_appId = appId;
    m_processName = processName;

    return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) NotificationListener::OnRawNotificationReceived(unsigned int payloadLength, _In_ byte* payload, _In_ HSTRING /*correlationVector*/) noexcept try
{
    auto lock = m_lock.lock_exclusive();

    winrt::com_array<uint8_t> payloadArray{ payload, payload + (payloadLength * sizeof(uint8_t)) };

    if (!m_foregroundSinkManager->InvokeForegroundHandlers(m_appId, payloadArray, payloadLength))
    {
        std::string commandLine = "----WindowsAppRuntimePushServer:-Payload:\"";
        commandLine.append(reinterpret_cast<char*>(payload), payloadLength);
        commandLine.append("\"");

        const std::string processNameAsUtf8String = ConvertProcessNameToUtf8String();

        SHELLEXECUTEINFOA shellExecuteInfo{};
        shellExecuteInfo.cbSize = sizeof(SHELLEXECUTEINFOA);
        shellExecuteInfo.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_DOENVSUBST;
        shellExecuteInfo.lpFile = processNameAsUtf8String.c_str();
        shellExecuteInfo.lpParameters = commandLine.c_str();

        shellExecuteInfo.nShow = SW_NORMAL;

        if (!ShellExecuteExA(&shellExecuteInfo))
        {
            THROW_IF_WIN32_ERROR(GetLastError());
        }
    };

    return S_OK;
}
CATCH_RETURN()

const std::string NotificationListener::ConvertProcessNameToUtf8String()
{
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, m_processName.c_str(), -1, NULL, 0, nullptr, nullptr);
    THROW_LAST_ERROR_IF(size_needed == 0);

    // size_needed minus the null character
    std::string utf8(size_needed - 1, 0);
    int size = WideCharToMultiByte(CP_UTF8, 0, m_processName.c_str(), size_needed - 1, &utf8[0], size_needed - 1, nullptr, nullptr);
    THROW_LAST_ERROR_IF(size == 0);
    return utf8;
}
