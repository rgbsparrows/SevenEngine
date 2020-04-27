#pragma once

#include "Macros/UtilMacros.h"
#include "Macros/ConditionDeprecated.h"
#include "Macros/BuildConfiguationMacro.h"

#include <string_view>

struct SBuildConfiguation
{
	static constexpr bool GIsWithDebugCode = WITH_DEBUG_CODE;
	static constexpr std::wstring_view GBuildConfig = MAKE_WIDE(MAKE_STR(SE_BUILD_CONFIG));
};
