#pragma once

#include "Core/Util/UtilMacros.h"
#include "Core/ProgramConfiguation/BuildConfiguation.h"
#include "Core/ProgramConfiguation/ProgramConfiguation.h"

#include <filesystem>

struct SBasicPath
{
	static std::filesystem::path GetBinarieSZTPath(const std::filesystem::path& _basicPath) noexcept { return _basicPath / "Binaries" / SBuildConfiguation::GBuildConfig; }

	static std::filesystem::path GetConfigPath(const std::filesystem::path& _basicPath) noexcept { return _basicPath / "Config"; }

	static std::filesystem::path GetPluginPath(const std::filesystem::path& _basicPath) noexcept { return _basicPath / "Plugins"; }

	static std::filesystem::path GetShaderPath(const std::filesystem::path& _basicPath) noexcept { return _basicPath / "Shaders"; }

	static std::filesystem::path GetSourcePath(const std::filesystem::path& _basicPath) noexcept { return _basicPath / "Source"; }

	static std::filesystem::path GetEnginePath() noexcept { return SProgramConfiguation::GetEnginePath(); }

	static std::filesystem::path GetEnginePluginPath() noexcept { return GetPluginPath(GetEnginePath()); }

	static std::filesystem::path GetEngineBinarieSZTPath() noexcept { return GetBinarieSZTPath(GetEnginePath()); }

	static std::filesystem::path GetEngineConfigPath() noexcept { return GetConfigPath(GetEnginePath()); }

	static std::filesystem::path GetEngineShaderPath() noexcept { return GetShaderPath(GetEnginePath()); }

	static std::filesystem::path GetEngineSourcePath() noexcept { return GetSourcePath(GetEnginePath()); }

	static std::filesystem::path GetProjectPath() noexcept { return SProgramConfiguation::GetProjectPath(); }

	static std::filesystem::path GetProjectPluginPath() noexcept { return GetPluginPath(GetProjectPath()); }
									
	static std::filesystem::path GetProjectBinarieSZTPath() noexcept { return GetBinarieSZTPath(GetProjectPath()); }
									
	static std::filesystem::path GetProjectConfigPath() noexcept { return GetConfigPath(GetProjectPath()); }
									
	static std::filesystem::path GetProjectShaderPath() noexcept { return GetShaderPath(GetProjectPath()); }
									
	static std::filesystem::path GetProjectSourcePath() noexcept { return GetSourcePath(GetProjectPath()); }
};
