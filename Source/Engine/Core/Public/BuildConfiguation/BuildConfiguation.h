#pragma once

#include "Macros/UtilMacros.h"
#include "Macros/ConditionDeprecated.h"
#include "Macros/BuildConfiguationMacro.h"

#include <string_view>

struct SBuildConfiguation
{
	static constexpr bool GIsWithDebugCode = WITH_DEBUG_CODE;
	static constexpr std::wstring_view GBuildPlatform = MAKE_WIDE(MAKE_STR(SBT_COMPILED_PLATFORM));
	static constexpr std::wstring_view GBuildTarget = MAKE_WIDE(MAKE_STR(SBT_COMPILED_TARGET));
	static constexpr std::wstring_view GPluginMainfestFileName = MAKE_WIDE(SBT_PLUGIN_MANIFEST);
};
