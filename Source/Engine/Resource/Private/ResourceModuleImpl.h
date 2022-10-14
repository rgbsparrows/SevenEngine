#pragma once

#include "Core/Container/Blob.h"
#include "Resource/ResourceModule.h"

#include <unordered_map>

constexpr uint64_t GResourceMagicNumber = 2718281828459045235;

class SResourceModuleImpl : public IResourceModule
{
public:
	virtual bool Init() noexcept override;
	virtual void Clear() noexcept override;
	virtual bool CreateResource(SResourceProxyBase _resourceProxy, const std::filesystem::path& _path) noexcept override;
	virtual bool RenameResource(SResourceProxyBase _resourceProxy, const std::filesystem::path& _newPath) noexcept override;
	virtual SResourceProxyBase LoadResource(const std::filesystem::path& _path) noexcept override;
	virtual void CreateDirectory(const std::filesystem::path& _path) noexcept;

public:
	void SaveResource(SResourceBase* _resource) noexcept;
	void RemoveResource(const std::filesystem::path& _path) noexcept;

private:
	SBlob MakeFileContent(SResourceBase* _resource) noexcept;

	void AddResource(const std::filesystem::path& _path, SResourceBase* _resource) noexcept;

	bool LoadFile(const std::filesystem::path& _path, SBlob& _blob) noexcept;
	bool SaveFile(SBufferView _content, const std::filesystem::path& _path, bool _overwrite) noexcept;

	std::filesystem::path ConvertPath(const std::filesystem::path& _path) noexcept;
	bool IsRawResource(SBufferView _content) noexcept;

private:
	//资源的持有权只由资源代理的持有者所拥有，所以资源模块内只存储资源指针，而不储存资源代理
	std::unordered_map<std::filesystem::path, SResourceBase*> mResourceMap;

};

SResourceModuleImpl* GetResourceModuleImpl() noexcept;
