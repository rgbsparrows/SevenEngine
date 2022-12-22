#pragma once

#include "Core/Container/Blob.h"
#include "Core/Class/ClassObject.h"
#include "Core/Misc/ReadWriteStream.h"

#include <filesystem>

struct SResourceDataHeader
{
	SClassIdentifier mResourceTypeHash = 0;
	uint64_t mContentSize = 0;
};

class SResourceBase
{
	DECLARE_ANCESTOR_CLASSOBJECT_BODY(SResourceBase)

public:
	std::filesystem::path GetResourcePath() const noexcept { return mResourcePath; }

	virtual void Serialize(SWriteStream& _writeStream)const noexcept {}
	virtual bool Deserialize(SReadStream& _readStream) { return true; }

	virtual void RefrashContent() noexcept {}

	bool IsDirty() const noexcept { return mIsDirty; }
	void MarkDirty() noexcept { mIsDirty = true; }
	void Save() noexcept;

	void Release() noexcept;

protected:
	virtual void OnRelease() noexcept;

public:
	void Internal_SetResourcePath(const std::filesystem::path& _filePath) noexcept { mResourcePath = _filePath; }
	void Internal_AddRef() noexcept { mRefCount++; }

private:
	std::filesystem::path mResourcePath;
	size_t mRefCount = 0;
	bool mIsDirty = false;
};
