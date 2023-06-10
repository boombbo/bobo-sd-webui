﻿// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#ifndef __WINDOWSAPPSDK_TEST_APPMODEL_H
#define __WINDOWSAPPSDK_TEST_APPMODEL_H

#include <appmodel.h>

#include <Verify.h>

namespace Test::AppModel
{
    inline bool IsPackagedProcess()
    {
        UINT32 n{};
        const auto rc = ::GetCurrentPackageFullName(&n, nullptr);
        VERIFY_IS_TRUE((rc == APPMODEL_ERROR_NO_PACKAGE) || (rc == ERROR_INSUFFICIENT_BUFFER));
        return rc == ERROR_INSUFFICIENT_BUFFER;
    }
}

#endif // __WINDOWSAPPSDK_TEST_APPMODEL_H
