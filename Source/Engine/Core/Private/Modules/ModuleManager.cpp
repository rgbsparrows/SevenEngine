#include "Core/Macros/Assert.h"
#include "Core/Path/BasicPath.h"
#include "Core/Logging/Logging.h"
#include "Core/Modules/ModuleManager.h"
#include "Core/Modules/ModuleInterface.h"
#include "Core/ProgramConfiguation/ProgramConfiguation.h"

#include <fstream>

void SModuleManager::Clear() noexcept
{
#if WITH_DEBUG_CODE
	for (auto& _ele : mModuleInfoMap)
		ASSERT(_ele.second.mRefCount == 0);
#endif
}

bool SModuleManager::LoadModule(std::wstring_view _moduleName) noexcept
{
	SModuleInfo* module = GetModuleInfo(_moduleName);

	CHECK(module != nullptr);

	if (module->mRefCount == 0)
	{
		module->mModule = (module->mModuleCreateFunc)();
		if (!module->mModule->Init())
			return false;
	}

	module->mRefCount++;

	return true;
}

void SModuleManager::UnloadModule(std::wstring_view _moduleName) noexcept
{
	SModuleInfo* module = GetModuleInfo(_moduleName);

	CHECK(module != nullptr);
	CHECK(module->mRefCount >= 1);

	if (module->mRefCount == 1)
	{
		module->mModule->Clear();
		module->mModule = nullptr;
	}

	module->mRefCount--;
}

ModuleDetail::SModuleRegister::SModuleRegister(std::wstring_view _moduleName, IModuleInterface* (*_moduleCreateFunc)() noexcept) noexcept
{
	SModuleManager::Get().RegistModule(_moduleName, _moduleCreateFunc);
}
