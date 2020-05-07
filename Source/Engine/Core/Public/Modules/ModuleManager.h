#pragma once

#include "Template/TemplateUtil.h"
#include "Macros/UtilMacros.h"

#include <string_view>
#include <filesystem>

__interface IModuleInterface;

using SModuleCreateFuncType = IModuleInterface * () noexcept;

class SModuleManager : public TAsSingleton<SModuleManager>
{
private:
	struct SModuleInfo
	{
		std::wstring_view mModuleName;
		SModuleCreateFuncType* mModuleCreateFunc = nullptr;
		IModuleInterface* mModule = nullptr;
		uint32_t mRefCount = 0;
	};

public:
	void RegistModule(std::wstring_view _moduleName, SModuleCreateFuncType* _moduleCreateFunc) noexcept
	{
		mModuleInfos.push_back(SModuleInfo{_moduleName, _moduleCreateFunc, nullptr, 0 });
	}

	void Init() noexcept;
	void Clear() noexcept;

	bool LoadModule(std::wstring_view _moduleName) noexcept;
	template<typename _moduleClass = IModuleInterface> auto GetModule(std::wstring_view _moduleName) noexcept { return static_cast<_moduleClass>(GetRawModule(_moduleName)); }
	void UnloadModule(std::wstring_view _moduleName) noexcept;

private:
	SModuleInfo* GetModuleInfo(std::wstring_view _moduleName) noexcept;
	IModuleInterface* GetRawModule(std::wstring_view _moduleName) noexcept;

private:

	std::vector<SModuleInfo> mModuleInfos;
};
