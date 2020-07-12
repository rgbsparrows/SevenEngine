#pragma once

#include "Core/Macros/Assert.h"
#include "Core/Macros/UtilMacros.h"
#include "Core/Template/TemplateUtil.h"

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
	void RegistModule(std::wstring_view _moduleName, SModuleCreateFuncType* _moduleCreateFunc) noexcept
	{
		ASSERT(_moduleName != L"" && _moduleCreateFunc!= nullptr);

		mModuleInfoMap[_moduleName] = SModuleInfo{ _moduleCreateFunc, nullptr, 0 };
	}

	void Init() noexcept {}
	void Clear() noexcept;

	bool LoadModule(std::wstring_view _moduleName) noexcept;
	template<typename _moduleClass = IModuleInterface> auto GetModule(std::wstring_view _moduleName) noexcept { return static_cast<_moduleClass>(GetRawModule(_moduleName)); }
	void UnloadModule(std::wstring_view _moduleName) noexcept;

private:
	SModuleInfo* GetModuleInfo(std::wstring_view _moduleName) noexcept
	{
		ASSERT(mModuleInfoMap.count(_moduleName) == 1);

		auto it = mModuleInfoMap.find(_moduleName);
		if (it == mModuleInfoMap.end()) return nullptr;
		else return &it->second;
	}

	IModuleInterface* GetRawModule(std::wstring_view _moduleName) noexcept
	{
		SModuleInfo* module = GetModuleInfo(_moduleName);
		return module ? module->mModule : nullptr;
	}

private:
	//���������moduleNameһ��������Դ�ڴ����ڳ��������ַ��������Կ���ֱ�Ӵ洢�����赣��������������
	std::map<std::wstring_view, SModuleInfo> mModuleInfoMap;
};