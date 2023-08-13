#pragma once

#include "Core/Util/Assert.h"
#include "Core/Util/UtilMacros.h"
#include "Core/Util/TemplateUtil.h"

#include <map>
#include <string_view>

__interface IModuleInterface;

using SModuleCreateFuncType = IModuleInterface * () noexcept;

class SModuleManager : public TAsSingleton<SModuleManager>
{
private:
	struct SModuleInfo
	{
		SModuleCreateFuncType* mModuleCreateFunc = nullptr;
		IModuleInterface* mModule = nullptr;
		uint32_t mRefCount = 0;
	};

public:
	void RegistModule(std::string_view _moduleName, SModuleCreateFuncType* _moduleCreateFunc) noexcept
	{
		ASSERT(_moduleName != "" && _moduleCreateFunc!= nullptr);

		mModuleInfoMap[_moduleName] = SModuleInfo{ _moduleCreateFunc, nullptr, 0 };
	}

	void Init() noexcept {}
	void Clear() noexcept;

	void LoadAllModule() noexcept;
	void UnloadAllModule() noexcept;

	bool LoadModule(std::string_view _moduleName) noexcept;
	template<typename _moduleClass = IModuleInterface> auto GetModule(std::string_view _moduleName) noexcept { return static_cast<_moduleClass>(GetRawModule(_moduleName)); }
	void UnloadModule(std::string_view _moduleName) noexcept;

private:
	SModuleInfo* GetModuleInfo(std::string_view _moduleName) noexcept
	{
		ASSERT(mModuleInfoMap.count(_moduleName) == 1);

		auto it = mModuleInfoMap.find(_moduleName);
		if (it == mModuleInfoMap.end()) return nullptr;
		else return &it->second;
	}

	IModuleInterface* GetRawModule(std::string_view _moduleName) noexcept
	{
		SModuleInfo* seModule = GetModuleInfo(_moduleName);
		return seModule ? seModule->mModule : nullptr;
	}

private:
	//���������moduleNameһ��������Դ�ڴ����ڳ��������ַ��������Կ���ֱ�Ӵ洢�����赣��������������
	std::map<std::string_view, SModuleInfo> mModuleInfoMap;
};
