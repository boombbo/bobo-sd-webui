﻿#pragma once

class PlatformLifetimeManager
{
public:

    PlatformLifetimeManager() {};

    void Setup();

    void Cancel();

    void Wait();

    void SignalEvent();

private:

    wil::srwlock m_lock;

    wil::unique_event m_event{ wil::EventOptions::None };
    wil::unique_threadpool_timer m_timer;
};
