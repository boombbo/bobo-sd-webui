﻿#pragma once

#include <unknwn.h>
#include <appmodel.h>

// Temporarily disable C4324 because WRL generates a false (well, irrelevant) warning
//   'Microsoft::WRL::Details::StaticStorage<Microsoft::WRL::Details::OutOfProcModuleBase<ModuleT>::GenericReleaseNotifier<T>,Microsoft::WRL::Details::StorageInstance::OutOfProcCallbackBuffer1,ModuleT>': structure was padded due to alignment specifier
/*#pragma warning(push)
#pragma warning(disable:4324)
#include <wrl.h>
#pragma warning(pop)*/

#include <wil/cppwinrt.h>
#include <wil/token_helpers.h>
#include <wil/stl.h>
#include <wil/resource.h>
#include <wil/result_macros.h>
#include <wil/filesystem.h>
#include <wil/com.h>
#include <wil/win32_helpers.h>
