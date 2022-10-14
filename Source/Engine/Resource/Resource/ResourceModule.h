#pragma once

#include "Resource/ResourceBase.h"
#include "Resource/ResourceProxy.h"
#include "Core/Modules/ModuleInterface.h"

#include <filesystem>

class SResourceBase;
struct IResourceModule : public IModuleInterface
{
	virtual bool CreateResource(SResourceProxyBase _resourceProxy, const std::filesystem::path& _path) noexcept = 0;
	virtual bool RenameResource(SResourceProxyBase _resourceProxy, const std::filesystem::path& _newPath) noexcept = 0;
	virtual SResourceProxyBase LoadResource(const std::filesystem::path& _path) noexcept = 0;

	virtual void CreateDirectory(const std::filesystem::path& _path) noexcept = 0;

	template<std::derived_from<SResourceBase> _resourceType>
	TResourceProxy<_resourceType> LoadResource(const std::filesystem::path& _path) noexcept
	{
		using ResourceType = _resourceType;
		using ResourceProxyType = TResourceProxy<ResourceType>;

		SResourceProxyBase resourceProxy = LoadResource(_path);

		if (!resourceProxy || IsInstanceOf<ResourceType>(resourceProxy.Get()) == false)
			return ResourceProxyType();
		
		return ResourceProxyType(resourceProxy);
	}
};

IResourceModule* GetResourceModule() noexcept;
