#include "Core/ProgramConfiguation/ProgramConfiguation.h"
#include "Core/Misc/CommandLine.h"

std::filesystem::path SProgramConfiguation::GetEnginePath() noexcept
{
	static std::filesystem::path enginePath = SCommandLine::GetArgumentValue<std::filesystem::path>(u8"EnginePath").second;
	return enginePath;
}

bool SProgramConfiguation::IsWithProject() noexcept
{
	static bool isWithProject = SCommandLine::HasArgument(u8"ProjectPath");
	return isWithProject;
}

std::filesystem::path SProgramConfiguation::GetProjectPath() noexcept
{
	static std::filesystem::path projectPath = SCommandLine::GetArgumentValue<std::filesystem::path>(u8"ProjectPath").second;
	return projectPath;
}

bool SProgramConfiguation::UseDebugShader() noexcept
{
	static bool useDebugShader = SCommandLine::GetArgumentValue<bool>(u8"UseDebugShader", false).second;
	return useDebugShader;
}

