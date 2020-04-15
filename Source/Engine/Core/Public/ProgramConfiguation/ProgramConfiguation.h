#pragma once

#include <filesystem>

struct SProgramConfiguation
{
	static std::filesystem::path GetEnginePath() noexcept;

	static bool IsWithProject()noexcept;

	static std::filesystem::path GetProjectPath() noexcept;
};
