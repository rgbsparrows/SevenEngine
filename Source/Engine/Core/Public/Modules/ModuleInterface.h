#pragma once

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
		SModuleRegister(std::wstring_view _moduleName, IModuleInterface* (*_moduleCreateFunc)() noexcept) noexcept;
	};
}

#define REGIST_MODULE(_moduleName, _moduleClass) ModuleDetail::SModuleRegister __##_moduleClass##_Register(_moduleName, []()->IModuleInterface* noexcept {return new _moduleClass;});
