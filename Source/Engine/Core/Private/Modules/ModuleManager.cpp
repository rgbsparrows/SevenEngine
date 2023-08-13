#include "Core/Util/Assert.h"
#include "Core/ProgramConfiguation/BasicPath.h"
#include "Core/Modules/ModuleManager.h"
#include "Core/Modules/ModuleInterface.h"
#include "Core/ProgramConfiguation/ProgramConfiguation.h"

#include <fstream>

void SModuleManager::Clear() noexcept
{
	if constexpr (SBuildConfiguation::GIsDebugMode)
	{
		for (auto& _ele : mModuleInfoMap)
			ASSERT(_ele.second.mRefCount == 0);
	}
}

void SModuleManager::LoadAllModule() noexcept
{
	for (const auto& module : mModuleInfoMap)
		LoadModule(module.first);
}

void SModuleManager::UnloadAllModule() noexcept
{
	for (const auto& module : mModuleInfoMap)
		UnloadModule(module.first);
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

ModuleDetail::SModuleRegister::SModuleRegister(std::string_view _moduleName, IModuleInterface* (*_moduleCreateFunc)() noexcept) noexcept
{
	SModuleManager::Get().RegistModule(_moduleName, _moduleCreateFunc);
}
