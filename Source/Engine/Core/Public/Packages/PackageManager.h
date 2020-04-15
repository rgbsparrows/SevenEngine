#pragma once

#include "Template/TemplateUtil.h"
#include "Macros/UtilMacros.h"

#include <string_view>
#include <filesystem>

__interface IModuleInterface;

using SModuleCreateFuncType = IModuleInterface * ();

struct SPluginInfo
{
	bool mIsEnable = false;
	std::filesystem::path mPluginPath;
	int32_t mFileVersion = 0;
	int32_t mVersion = 0;
	std::wstring mVersionName;
	std::wstring mFriendlyName;
	std::wstring mDescription;
	std::wstring mCategory;
	std::wstring mCreatedBy;
	std::wstring mCreatedByURL;
	std::wstring mDocsURL;
	std::wstring mMarketplaceURL;
	std::wstring mSupportURL;
	std::wstring mEngineVersion;
};

class SPackageManager : public TAsSingleton<SPackageManager>
{
private:
	struct SModuleInfo
	{
		std::wstring_view mPackageName;
		std::wstring_view mModuleName;
		SModuleCreateFuncType* mModuleCreateFunc = nullptr;
		IModuleInterface* mModule = nullptr;
		uint32_t mRefCount = 0;
	};

public:
	static constexpr std::wstring_view GamePackageName = L"Game";
	static constexpr std::wstring_view EnginePackageName = L"Engine";

	void RegistModule(std::wstring_view _packageName, std::wstring_view _moduleName, SModuleCreateFuncType* _moduleCreateFunc)
	{
		mModuleInfos.push_back(SModuleInfo{ _packageName, _moduleName, _moduleCreateFunc, nullptr, 0 });
	}

	void Init();
	void Clear();

	void EnablePackage(std::wstring_view _packageName);
	void DisablePackage(std::wstring_view _packageName);
	bool IsPackageEnable(std::wstring_view _packageName);

	std::filesystem::path GetPackagePath(std::wstring_view _packageName);
	bool IsPlugin(std::wstring_view _pluginName) { return IsEnginePlugin(_pluginName) || IsGamePlugin(_pluginName); }
	bool IsEnginePlugin(std::wstring_view _pluginName) { return GetEnginePlugin(_pluginName) != nullptr; }
	bool IsGamePlugin(std::wstring_view _pluginName) { return mWithGamePackage && GetGamePlugin(_pluginName) != nullptr; }
	bool GetPluginDesc(std::wstring_view _pluginName, SPluginInfo& _desc);

	bool IsEnginePackage(std::wstring_view _packageName) { return _packageName == EnginePackageName || IsEnginePlugin(_packageName); }
	bool IsGamePackage(std::wstring_view _packageName) { return mWithGamePackage && (_packageName == GamePackageName || GetGamePlugin(_packageName)); }

	bool LoadModule(std::wstring_view _moduleName);
	template<typename _moduleClass = IModuleInterface>
	auto GetModule(std::wstring_view _moduleName) { return static_cast<_moduleClass>(GetRawModule(_moduleName)); }
	void UnloadModule(std::wstring_view _moduleName);

private:
	void LoadPluginManifestFile(std::filesystem::path _pluginManifestFile, std::vector<SPluginInfo>& _pluginInfo);
	SPluginInfo* GetPlugin(std::wstring_view _pluginName);
	SPluginInfo* GetEnginePlugin(std::wstring_view _pluginName);
	SPluginInfo* GetGamePlugin(std::wstring_view _pluginName);
	SModuleInfo* GetModuleInfo(std::wstring_view _moduleName);

	IModuleInterface* GetRawModule(std::wstring_view _moduleName);

private:
	bool mWithGamePackage = true;

	std::vector<SPluginInfo> mEnginePlugins;
	std::vector<SPluginInfo> mProjectPlugins;
	std::vector<SModuleInfo> mModuleInfos;
};
