#include "Packages/PackageManager.h"
#include "Macros/Assert.h"
#include "Path/BasicPath.h"
#include "Logging/Logging.h"
#include "Packages/ModuleInterface.h"
#include "ProgramConfiguation/ProgramConfiguation.h"

#include <fstream>

void SPackageManager::Init()
{
	mWithGamePackage = SProgramConfiguation::IsWithProject();

	LoadPluginManifestFile(SBasicPath::GetEnginePlatformBinaryPath() / SBuildConfiguation::GPluginMainfestFileName, mEnginePlugins);
	if (mWithGamePackage)
		LoadPluginManifestFile(SBasicPath::GetProjectPlatformBinaryPath() / SBuildConfiguation::GPluginMainfestFileName, mProjectPlugins);
}

void SPackageManager::Clear()
{
#if WITH_DEBUG_CODE
	for (const auto& _module : mModuleInfos)
		CHECK(_module.mRefCount == 0);
#endif
}

void SPackageManager::EnablePackage(std::wstring_view _packageName)
{
	if (_packageName == EnginePackageName || (mWithGamePackage && _packageName == GamePackageName)) return;

	SPluginInfo* plugin = GetPlugin(_packageName);

	CHECK(plugin != nullptr && L"package not found");

#if WITH_DEVELOPMENT_CODE
	if (plugin->mIsEnable)
	{
		LogWarn(L"plugin[", std::wstring(_packageName.begin(), _packageName.end()), L"] has been enabled");
	}
#endif

	plugin->mIsEnable = true;
}

void SPackageManager::DisablePackage(std::wstring_view _packageName)
{
	CHECK(_packageName != GamePackageName && _packageName != EnginePackageName);

	SPluginInfo* plugin = GetPlugin(_packageName);

	CHECK(plugin != nullptr && L"package not found");

#if WITH_DEVELOPMENT_CODE
	if (plugin->mIsEnable)
		LogWarn(L"plugin[", std::wstring(_packageName.begin(), _packageName.end()), L"] has been disabled");
#endif

#if WITH_DEBUG_CODE
	for (auto& _module : mModuleInfos)
		if (_module.mPackageName == _packageName)
			CHECK(_module.mRefCount == 0);
#endif 

	plugin->mIsEnable = false;
}

bool SPackageManager::IsPackageEnable(std::wstring_view _packageName)
{
	if (_packageName == EnginePackageName || (mWithGamePackage && _packageName == GamePackageName)) return true;

	SPluginInfo* plugin = GetPlugin(_packageName);

	CHECK(plugin != nullptr && L"package not found");

	return plugin->mIsEnable;
}

std::filesystem::path SPackageManager::GetPackagePath(std::wstring_view _packageName)
{
	if (_packageName == EnginePackageName) return SBasicPath::GetEnginePath();
	if (mWithGamePackage && _packageName == GamePackageName) return SBasicPath::GetProjectPath();

	SPluginInfo* plugin = GetPlugin(_packageName);

	CHECK(plugin != nullptr && L"package not found");

	return plugin->mPluginPath;
}

bool SPackageManager::GetPluginDesc(std::wstring_view _pluginName, SPluginInfo& _desc)
{
	SPluginInfo* plugin = GetPlugin(_pluginName);

	CHECK(plugin != nullptr && L"package not found");

	_desc = *plugin;

	return true;
}

bool SPackageManager::LoadModule(std::wstring_view _moduleName)
{
	SModuleInfo* module = GetModuleInfo(_moduleName);

	CHECK(module != nullptr);

	if (!IsPackageEnable(module->mPackageName)) return false;

	if (module->mRefCount == 0)
	{
		module->mModule = (module->mModuleCreateFunc)();
		module->mModule->Init();
	}

	module->mRefCount++;

	return true;
}

void SPackageManager::UnloadModule(std::wstring_view _moduleName)
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

void SPackageManager::LoadPluginManifestFile(std::filesystem::path _pluginManifestFile, std::vector<SPluginInfo>& _pluginInfo)
{
	REWRITE_WHEN_SE_STREAM_AVAILABLE("")
		std::ifstream ifs(_pluginManifestFile, std::ios::in | std::ios::binary);

	CHECK(!ifs.bad());

	uint64_t pluginCount;
	ifs.read(reinterpret_cast<char*>(&pluginCount), sizeof(uint64_t));

	for (uint64_t i = 0; i != pluginCount; ++i)
	{
		SPluginInfo pluginInfo;

		std::wstring directory;
		uint16_t strSize;

		ifs.read(reinterpret_cast<char*>(&strSize), sizeof(uint16_t));
		directory.resize(strSize);
		ifs.read(reinterpret_cast<char*>(&directory[0]), static_cast<std::streamsize>(strSize) * sizeof(uint16_t));

		ifs.read(reinterpret_cast<char*>(&pluginInfo.mFileVersion), sizeof(int32_t));
		ifs.read(reinterpret_cast<char*>(&pluginInfo.mVersion), sizeof(int32_t));

		ifs.read(reinterpret_cast<char*>(&strSize), sizeof(uint16_t));
		pluginInfo.mVersionName.resize(strSize);
		ifs.read(reinterpret_cast<char*>(&pluginInfo.mVersionName[0]), static_cast<std::streamsize>(strSize) * sizeof(uint16_t));

		ifs.read(reinterpret_cast<char*>(&strSize), sizeof(uint16_t));
		pluginInfo.mFriendlyName.resize(strSize);
		ifs.read(reinterpret_cast<char*>(&pluginInfo.mFriendlyName[0]), static_cast<std::streamsize>(strSize) * sizeof(uint16_t));

		ifs.read(reinterpret_cast<char*>(&strSize), sizeof(uint16_t));
		pluginInfo.mDescription.resize(strSize);
		ifs.read(reinterpret_cast<char*>(&pluginInfo.mDescription[0]), static_cast<std::streamsize>(strSize) * sizeof(uint16_t));

		ifs.read(reinterpret_cast<char*>(&strSize), sizeof(uint16_t));
		pluginInfo.mCategory.resize(strSize);
		ifs.read(reinterpret_cast<char*>(&pluginInfo.mCategory[0]), static_cast<std::streamsize>(strSize) * sizeof(uint16_t));

		ifs.read(reinterpret_cast<char*>(&strSize), sizeof(uint16_t));
		pluginInfo.mCreatedBy.resize(strSize);
		ifs.read(reinterpret_cast<char*>(&pluginInfo.mCreatedBy[0]), static_cast<std::streamsize>(strSize) * sizeof(uint16_t));

		ifs.read(reinterpret_cast<char*>(&strSize), sizeof(uint16_t));
		pluginInfo.mCreatedByURL.resize(strSize);
		ifs.read(reinterpret_cast<char*>(&pluginInfo.mCreatedByURL[0]), static_cast<std::streamsize>(strSize) * sizeof(uint16_t));

		ifs.read(reinterpret_cast<char*>(&strSize), sizeof(uint16_t));
		pluginInfo.mDocsURL.resize(strSize);
		ifs.read(reinterpret_cast<char*>(&pluginInfo.mDocsURL[0]), static_cast<std::streamsize>(strSize) * sizeof(uint16_t));

		ifs.read(reinterpret_cast<char*>(&strSize), sizeof(uint16_t));
		pluginInfo.mMarketplaceURL.resize(strSize);
		ifs.read(reinterpret_cast<char*>(&pluginInfo.mMarketplaceURL[0]), static_cast<std::streamsize>(strSize) * sizeof(uint16_t));

		ifs.read(reinterpret_cast<char*>(&strSize), sizeof(uint16_t));
		pluginInfo.mSupportURL.resize(strSize);
		ifs.read(reinterpret_cast<char*>(&pluginInfo.mSupportURL[0]), static_cast<std::streamsize>(strSize) * sizeof(uint16_t));

		ifs.read(reinterpret_cast<char*>(&strSize), sizeof(uint16_t));
		pluginInfo.mEngineVersion.resize(strSize);
		ifs.read(reinterpret_cast<char*>(&pluginInfo.mEngineVersion[0]), static_cast<std::streamsize>(strSize) * sizeof(uint16_t));

		pluginInfo.mPluginPath = directory;

		_pluginInfo.push_back(std::move(pluginInfo));
	}

	CHECK(!ifs.bad());
}

SPluginInfo* SPackageManager::GetPlugin(std::wstring_view _pluginName)
{
	SPluginInfo* plugin = nullptr;
	plugin = GetEnginePlugin(_pluginName);
	if (plugin == nullptr)
		plugin = GetGamePlugin(_pluginName);

	return plugin;
}

SPluginInfo* SPackageManager::GetEnginePlugin(std::wstring_view _pluginName)
{
	for (auto& _plugin : mEnginePlugins)
		if (_plugin.mFriendlyName == _pluginName)
			return &_plugin;

	return nullptr;
}

SPluginInfo* SPackageManager::GetGamePlugin(std::wstring_view _pluginName)
{
	if (!mWithGamePackage) return nullptr;

	for (auto& _plugin : mProjectPlugins)
		if (_plugin.mFriendlyName == _pluginName)
			return &_plugin;

	return nullptr;
}

SPackageManager::SModuleInfo* SPackageManager::GetModuleInfo(std::wstring_view _moduleName)
{
	for (auto& _module : mModuleInfos)
		if (_module.mModuleName == _moduleName)
			return &_module;

	return nullptr;
}

IModuleInterface* SPackageManager::GetRawModule(std::wstring_view _moduleName)
{
	SModuleInfo* module = GetModuleInfo(_moduleName);

	CHECK(module != nullptr);

	return module->mModule;
}
