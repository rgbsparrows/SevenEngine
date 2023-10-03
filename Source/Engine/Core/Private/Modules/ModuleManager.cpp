#include "Core/Util/Assert.h"
#include "Core/Misc/ConfigFile.h"
#include "Core/Modules/ModuleManager.h"
#include "Core/Modules/ModuleInterface.h"
#include "Core/ProgramConfiguation/BasicPath.h"
#include "Core/ProgramConfiguation/ProgramConfiguation.h"

#include <fstream>

void SModuleManager::Init() noexcept
{
	if (std::shared_ptr<SConfigFile> moduleConfig = SConfigFile::LoadConfigFile(SBasicPath::GetEngineConfigPath() / "ModuleConfig.json"))
	{
		std::vector<std::string> enableModuleList;
		std::vector<std::string> disableModuleList;

		moduleConfig->GetValue("Module", "EnableModuleList", enableModuleList);
		moduleConfig->GetValue("Module", "DisableModuleList", disableModuleList);

		for (const std::string& enableModuleName : enableModuleList)
		{
			if (SModuleInfo* moduleInfo = GetModuleInfo(enableModuleName))
			{
				moduleInfo->mEnableDefault = true;
			}
		}

		for (const std::string& disableModuleName : disableModuleList)
		{
			if (SModuleInfo* moduleInfo = GetModuleInfo(disableModuleName))
			{
				moduleInfo->mEnableDefault = false;
			}
		}
	}
}

void SModuleManager::Clear() noexcept
{
	if constexpr (SBuildConfiguation::GIsDebugMode)
	{
		for (auto& _ele : mModuleInfoMap)
			ASSERT(_ele.second.mRefCount == 0);
	}
}

void SModuleManager::LoadAllEnableModule() noexcept
{
	for (const auto& module : mModuleInfoMap)
	{
		if (module.second.mEnableDefault)
			LoadModule(module.first);
	}
}

void SModuleManager::UnloadAllEnableModule() noexcept
{
	for (const auto& module : mModuleInfoMap)
	{
		if (module.second.mEnableDefault)
			UnloadModule(module.first);
	}
}

bool SModuleManager::LoadModule(std::string_view _moduleName) noexcept
{
	SModuleInfo* seModule = GetModuleInfo(_moduleName);

	CHECK(seModule != nullptr);

	if (seModule->mRefCount == 0)
	{
		seModule->mModule = (seModule->mModuleCreateFunc)();
		if (!seModule->mModule->Init())
			return false;
	}

	seModule->mRefCount++;

	return true;
}

void SModuleManager::UnloadModule(std::string_view _moduleName) noexcept
{
	SModuleInfo* seModule = GetModuleInfo(_moduleName);

	CHECK(seModule != nullptr);
	CHECK(seModule->mRefCount >= 1);

	if (seModule->mRefCount == 1)
	{
		seModule->mModule->Clear();
		seModule->mModule = nullptr;
	}

	seModule->mRefCount--;
}

ModuleDetail::SModuleRegister::SModuleRegister(std::string_view _moduleName, IModuleInterface* (*_moduleCreateFunc)() noexcept, bool _enableDefault) noexcept
{
	SModuleManager::Get().RegistModule(_moduleName, _moduleCreateFunc, _enableDefault);
}
