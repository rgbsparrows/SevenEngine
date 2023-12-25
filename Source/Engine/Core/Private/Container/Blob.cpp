#include "Core/Util/Assert.h"
#include "Core/Container/Blob.h"

#include <vcruntime_new.h>

SBlob::SBlob(size_t _bufferSize) noexcept
{
	if (_bufferSize == 0) return;

	mBuffer = operator new(_bufferSize, std::nothrow);
	mBufferSize = _bufferSize;
}

SBlob::SBlob(const void* _buffer, size_t _bufferSize) noexcept
{
	if (_bufferSize == 0) return;
	ASSERT(_buffer != nullptr);

	mBuffer = operator new(_bufferSize);
	mBufferSize = _bufferSize;

	Memcpy(SBufferView(*this), _buffer, _bufferSize);
}

SBlob::SBlob(SBlob&& _blob) noexcept
{
	mBuffer = _blob.mBuffer;
	mBufferSize = _blob.mBufferSize;

	_blob.mBuffer = nullptr;
	_blob.mBufferSize = 0;
}

SBlob::~SBlob() noexcept
{
	if (mBuffer)
		operator delete(mBuffer, std::nothrow);

	mBuffer = nullptr;
	mBufferSize = 0;
}
