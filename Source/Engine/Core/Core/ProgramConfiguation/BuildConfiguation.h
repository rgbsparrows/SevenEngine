#pragma once

#include "Core/Util/UtilMacros.h"
#include "Core/ProgramConfiguation/ConditionDeprecated.h"
#include "Core/ProgramConfiguation/BuildConfiguationMacro.h"

#include <string_view>

struct SBuildConfiguation
{
	static constexpr bool GIsWithDebugCode = WITH_DEBUG_CODE;
	static constexpr bool GIsDebugMode = WITH_DEBUG_CODE;
	static constexpr std::string_view GBuildConfig = MAKE_STR(SE_BUILD_CONFIG);
};
