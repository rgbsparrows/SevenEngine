#include "Modules/ModuleManager.h"
#include "Macros/Assert.h"
#include "Path/BasicPath.h"
#include "Logging/Logging.h"
#include "Modules/ModuleInterface.h"
#include "ProgramConfiguation/ProgramConfiguation.h"

#include <fstream>


void SModuleManager::Init() noexcept
{
#if WITH_DEBUG_CODE
	std::sort(mModuleInfos.begin(), mModuleInfos.end(), [](const SModuleInfo& _left, const SModuleInfo& _right) {return _left.mModuleName < _right.mModuleName; });
	if (mModuleInfos.size() > 1)
	{
		for (size_t i = 1; i != mModuleInfos.size(); ++i)
			CHECK(mModuleInfos[i].mModuleName != mModuleInfos[i - 1].mModuleName);
	}
#endif
}

void SModuleManager::Clear() noexcept
{
#if WITH_DEBUG_CODE
	for (const auto& _module : mModuleInfos)
		CHECK(_module.mRefCount == 0);
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

SModuleManager::SModuleInfo* SModuleManager::GetModuleInfo(std::wstring_view _moduleName) noexcept
{
	for (auto& _module : mModuleInfos)
		if (_module.mModuleName == _moduleName)
			return &_module;

	return nullptr;
}

IModuleInterface* SModuleManager::GetRawModule(std::wstring_view _moduleName) noexcept
{
	SModuleInfo* module = GetModuleInfo(_moduleName);

	CHECK(module != nullptr);

	return module->mModule;
}

ModuleDetail::SModuleRegister::SModuleRegister(std::wstring_view _moduleName, IModuleInterface* (*_moduleCreateFunc)() noexcept) noexcept
{
	SModuleManager::Get().RegistModule(_moduleName, _moduleCreateFunc);
}