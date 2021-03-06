#pragma once

#include "BuildConfiguationMacro.h"
#include "Core/Macros/UtilMacros.h"
#include "Core/windowsEx.h"

#define CHECK(_expr) do { if (!(_expr)) _wassert(MAKE_WIDE(MAKE_STR(#_expr)), MAKE_WIDE(__FILE__), __LINE__); } while (0)

#if WITH_DEBUG_CODE
#define ASSERT(_expr) CHECK(_expr)
#else
#define ASSERT(_expr)
#endif
