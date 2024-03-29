#pragma once

#include <string_view>

__interface IModuleInterface
{
	bool Init() noexcept;
	void Clear() noexcept;
};

struct SDefaultModule : IModuleInterface
{
	bool Init() noexcept override {}
	void Clear() noexcept override {}
};

namespace ModuleDetail
{
	struct SModuleRegister
	{
		SModuleRegister(std::string_view _moduleName, IModuleInterface* (*_moduleCreateFunc)() noexcept, bool _enableDefault) noexcept;
	};
}

#define REGIST_MODULE(_moduleName, _moduleClass) ModuleDetail::SModuleRegister __##_moduleClass##_Register(_moduleName, []() noexcept->IModuleInterface* {return new _moduleClass;}, true);
#define REGIST_DISABLE_MODULE(_moduleName, _moduleClass) ModuleDetail::SModuleRegister __##_moduleClass##_Register(_moduleName, []() noexcept->IModuleInterface* {return new _moduleClass;}, false);
