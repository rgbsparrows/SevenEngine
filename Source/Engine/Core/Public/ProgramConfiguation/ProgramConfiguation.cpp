#include "ProgramConfiguation.h"
#include "CommandLine/CommandLine.h"

std::filesystem::path SProgramConfiguation::GetEnginePath() noexcept
{
	static std::filesystem::path enginePath = SCommandLine::GetArgumentValue<std::filesystem::path>(L"EnginePath").second;
	return enginePath;
}

bool SProgramConfiguation::IsWithProject() noexcept
{
	static bool isWithProject = SCommandLine::HasArgument(L"ProjectPath");
	return isWithProject;
}

std::filesystem::path SProgramConfiguation::GetProjectPath() noexcept
{
	static std::filesystem::path projectPath = SCommandLine::GetArgumentValue<std::filesystem::path>(L"ProjectPath").second;
	return projectPath;
}
