#pragma once

#include "Resource/ResourceBase.h"
#include "Core/Modules/ModuleInterface.h"

#include <filesystem>

class SResourceBase;

class IResourceModule : public IModuleInterface
{
	virtual bool Init() noexcept = 0;
	virtual void Clear() noexcept = 0;

	virtual bool CreateResource(SResourceBase* _resource, const std::filesystem::path& _path) noexcept = 0;
	virtual bool RenameResource(SResourceBase* _resource, const std::filesystem::path& _newPath) noexcept = 0;
	virtual SResourceBase* LoadResource(const std::filesystem::path& _path) noexcept = 0;

	virtual void CreateDirectory(const std::filesystem::path& _path) noexcept = 0;

	template<std::derived_from<SResourceBase> _resourceType>
	_resourceType* LoadResource(const std::filesystem::path& _path) noexcept
	{
		using ResourceType = _resourceType;
		SResourceBase* resource = LoadResource(_path);

		if (resource == nullptr)
			return nullptr;

		if(IsInstanceOf<ResourceType>(resource) == false)
		{
			resource->Release();
			return nullptr;
		}
		
		return static_cast<ResourceType*>(resource);
	}
};

IResourceModule* GetResourceModule() noexcept;
