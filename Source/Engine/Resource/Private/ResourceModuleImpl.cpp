#include "Core/Util/Assert.h"
#include "ResourceModuleImpl.h"
#include "Resource/ResourceBase.h"
#include "Core/Class/ClassManager.h"
#include "Core/Misc/ReadWriteStream.h"
#include "Resource/ResourceType/RawResource.h"
#include "Core/ProgramConfiguation/BasicPath.h"

#include <fstream>

SResourceModuleImpl* GResourceModuleImpl = nullptr;

TODO("功能可以抄一下Unity的AssetDataBase")
IResourceModule* GetResourceModule() noexcept
{
	return GResourceModuleImpl;
}

SResourceModuleImpl* GetResourceModuleImpl() noexcept
{
	return GResourceModuleImpl;
}

REGIST_MODULE("ResourceModule", SResourceModuleImpl)

bool SResourceModuleImpl::Init() noexcept
{
	GResourceModuleImpl = this;
	return true;
}

void SResourceModuleImpl::Clear() noexcept
{
	CHECK(mResourceMap.size() == 0);
}

bool SResourceModuleImpl::CreateResource(SResourceProxyBase _resourceProxy, const std::filesystem::path& _path) noexcept
{
	CHECK(!!_resourceProxy);
	CHECK(_path.empty() == false);
	CHECK(_resourceProxy->GetResourcePath().empty());

	std::filesystem::path path = _path.lexically_normal();

	bool res = SaveFile(MakeFileContent(_resourceProxy.Get()), path, false);

	if (res)
	{
		AddResource(path, _resourceProxy.Get());
		_resourceProxy->Internal_SetResourcePath(path);
	}

	return res;
}

bool SResourceModuleImpl::RenameResource(SResourceProxyBase _resourceProxy, const std::filesystem::path& _newPath) noexcept
{
	CHECK(!!_resourceProxy);
	CHECK(_newPath.empty() == false);
	CHECK(_resourceProxy->GetResourcePath().empty() == false);

	std::filesystem::path newPath = _newPath.lexically_normal();

	RemoveResource(_resourceProxy->GetResourcePath());
	AddResource(newPath, _resourceProxy.Get());
	_resourceProxy->Internal_SetResourcePath(newPath);

	return true;
}

SResourceProxyBase SResourceModuleImpl::LoadResource(const std::filesystem::path& _path) noexcept
{
	CHECK(_path.empty() == false);

	std::filesystem::path path = _path.lexically_normal();

	auto resourceIt = mResourceMap.find(path);

	if (resourceIt != mResourceMap.end())
	{
		resourceIt->second->Internal_AddRef();
		return SResourceProxyBase(resourceIt->second);
	}

	SBlob fileContent;
	bool res = LoadFile(path, fileContent);

	if (res == false)
		return SResourceProxyBase();

	SReadStream readStream(fileContent);

	SResourceBase* resource = nullptr;

	if (IsRawResource(fileContent))
	{
		resource = new SRawResource;
		resource->Internal_SetResourcePath(path);
		resource->Deserialize(readStream);
		resource->RefrashContent();
	}
	else
	{
		CHECK(readStream.Read<uint64_t>() == GResourceMagicNumber);
		SResourceDataHeader resourceDataHeader = readStream.Read<SResourceDataHeader>();
		resource = SClassManager::Get().ConstructObject<SResourceBase>(resourceDataHeader.mResourceTypeHash);
		if (resource != nullptr)
		{
			SReadStream subReadStream = readStream.MakeSubReadStream(resourceDataHeader.mContentSize);
			resource->Internal_SetResourcePath(path);
			resource->Deserialize(subReadStream);
			resource->RefrashContent();
		}
	}

	if (resource != nullptr)
		AddResource(path, resource);

	return SResourceProxyBase(resource);
}

void SResourceModuleImpl::CreateDirectory(const std::filesystem::path& _path) noexcept
{
	CHECK(std::filesystem::exists(_path) == false);

	std::filesystem::create_directories(_path);
}

void SResourceModuleImpl::SaveResource(SResourceBase* _resource) noexcept
{
	SaveFile(MakeFileContent(_resource), _resource->GetResourcePath(), true);
}

void SResourceModuleImpl::RemoveResource(const std::filesystem::path& _path) noexcept
{
	CHECK(_path.empty() == false);
	mResourceMap.erase(_path);
}

SBlob SResourceModuleImpl::MakeFileContent(SResourceBase* _resource) noexcept
{
	CHECK(!!_resource);

	SWriteStream writeStream;

	if (IsInstanceOf<SRawResource>(_resource) == false)
	{
		writeStream.Write(GResourceMagicNumber);
		writeStream.Write(SResourceDataHeader());
	}

	size_t resourceByteCount = 0;
	{
		SWriteStreamStat stat(resourceByteCount, writeStream);

		_resource->Serialize(writeStream);
	}

	if (IsInstanceOf<SRawResource>(_resource) == false)
	{
		writeStream.Seek(sizeof(uint64_t));
		SResourceDataHeader resourceHeader;
		resourceHeader.mResourceTypeHash = GetClassHash(_resource);
		resourceHeader.mContentSize = resourceByteCount;
		writeStream.Write(resourceHeader);
	}

	return SBlob(writeStream.GetContentBuffer().GetBuffer(), writeStream.GetContentBuffer().GetBufferSize());
}

void SResourceModuleImpl::AddResource(const std::filesystem::path& _path, SResourceBase* _resource) noexcept
{
	CHECK(_path.empty() == false);
	CHECK(mResourceMap.count(_path) == 0);
	mResourceMap.insert(std::make_pair(_path, _resource));
}

bool SResourceModuleImpl::LoadFile(const std::filesystem::path& _path, SBlob& _blob) noexcept
{
	_blob = SBlob();

	std::ifstream ifs(ConvertPath(_path), std::ios_base::binary);

	if (ifs.is_open() == false)
		return false;

	ifs.seekg(0, std::ios_base::end);
	size_t fileLen = ifs.tellg();
	ifs.seekg(0, std::ios_base::beg);

	_blob = SBlob(fileLen);
	ifs.read(reinterpret_cast<char*>(_blob.GetBuffer()), _blob.GetBufferSize());

	ifs.close();

	return true;
}

bool SResourceModuleImpl::SaveFile(SBufferView _content, const std::filesystem::path& _path, bool _overwrite) noexcept
{
	std::filesystem::path path = ConvertPath(_path);

	if (!_overwrite && std::filesystem::exists(path))
		return false;

	if (std::filesystem::exists(path.parent_path()) == false)
		std::filesystem::create_directories(path.parent_path());

	std::ofstream ofs(path, std::ios_base::binary | std::ios_base::trunc);

	if (ofs.is_open() == false)
		return false;

	ofs.write(reinterpret_cast<const char*>(_content.GetBuffer()), _content.GetBufferSize());
	ofs.close();

	return true;
}

std::filesystem::path SResourceModuleImpl::ConvertPath(const std::filesystem::path& _path) noexcept
{
	if (_path.empty())
		return _path;

	std::filesystem::path path;

	auto it = _path.begin();

	if ((*it) == "Engine")
		path = SBasicPath::GetEnginePath();
	else if ((*it) == "Game")
		path = SBasicPath::GetProjectPath();
	else
		path = *it;

	for (++it; it != _path.end(); ++it)
		path /= *it;

	return path;
}

bool SResourceModuleImpl::IsRawResource(SBufferView _content) noexcept
{
	SReadStream readStream(_content);

	if (readStream.GetContentSize() < sizeof(uint64_t) + sizeof(SResourceDataHeader))
		return false;

	return GResourceMagicNumber != readStream.Read<uint64_t>();
}
