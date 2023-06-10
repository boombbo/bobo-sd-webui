﻿// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the project root for license information.

#include "pch.h"

#include "appmodel_packageinfo.h"
#include "MddDetourPackageGraph.h"

LONG appmodel::GetPackageInfo2(
    PACKAGE_INFO_REFERENCE packageInfoReference,
    UINT32 flags,
    PackagePathType packagePathType,
    UINT32* bufferLength,
    BYTE* buffer,
    UINT32* count)
{
    return MddGetPackageInfo1Or2(packageInfoReference, flags, packagePathType, bufferLength, buffer, count);
}
