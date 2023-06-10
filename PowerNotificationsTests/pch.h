﻿// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here

#ifndef INLINE_TEST_METHOD_MARKUP
#define INLINE_TEST_METHOD_MARKUP
#endif

#include "framework.h"
#include <WexTestClass.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Management.Deployment.h>


#include <ProjectReunion.Test.Bootstrap.h>

#endif //PCH_H
