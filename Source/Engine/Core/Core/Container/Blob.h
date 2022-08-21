#pragma once

#include <tuple>
#include <vector>

template<typename ..._elementTypes>
using PackageVector = std::vector<std::tuple<_elementTypes...>>;

class SBlob
{
public:
	SBlob() noexcept = default;
	SBlob(size_t _bufferSize) noexcept;
	SBlob(const void* _buffer, size_t _bufferSize) noexcept;

	SBlob(const SBlob& _blob) noexcept :SBlob(_blob.GetBuffer(), _blob.GetBufferSize()) {}
	SBlob(SBlob&& _blob) noexcept;
	~SBlob() noexcept;

	const SBlob& operator=(const SBlob& _blob) noexcept
	{
		if (this != &_blob)
		{
			this->~SBlob();
			new(this) SBlob(_blob);
		}
	}

	const SBlob& operator=(SBlob&& _blob) noexcept
	{
		if (this != &_blob)
		{
			this->~SBlob();
			new(this) SBlob(std::forward<SBlob>(_blob));
		}

		return *this;
	}

	void ResizeBlob(size_t _bufferSize) noexcept
	{
		this->~SBlob();
		new(this) SBlob(_bufferSize);
	}

	void ResizeBlob(const void* _buffer, size_t _bufferSize) noexcept
	{
		this->~SBlob();
		new(this) SBlob(_buffer, _bufferSize);
	}

	void* GetBuffer() noexcept { return mBuffer; }
	const void* GetBuffer() const noexcept { return mBuffer; }
	bool IsEmpty() const noexcept { return mBuffer != nullptr; }
	size_t GetBufferSize() const noexcept { return mBufferSize; }

private:
	void* mBuffer = nullptr;
	size_t mBufferSize = 0;
};

class SBufferView
{
public:
	SBufferView() noexcept = default;
	SBufferView(const SBufferView&) noexcept = default;

	SBufferView(const void* _buffer, size_t _bufferSize) noexcept
		: mBuffer(_buffer), mBufferSize(_bufferSize)
	{
	}

	SBufferView(const void* _buffer, size_t _offset, size_t _bufferSize) noexcept
		: mBuffer(static_cast<const uint8_t*>(_buffer) + _offset), mBufferSize(_bufferSize)
	{
	}

	SBufferView(const SBlob& _blob) noexcept
		: SBufferView(_blob.GetBuffer(), _blob.GetBufferSize())
	{
	}

	SBufferView(const SBlob& _blob, size_t _offset) noexcept
		: SBufferView(_blob.GetBuffer(), _offset, _blob.GetBufferSize() - _offset)
	{
	}

	SBufferView(const SBlob& _blob, size_t _offset, size_t _bufferSize) noexcept
		: SBufferView(_blob.GetBuffer(), _offset, _bufferSize)
	{
	}

	SBufferView(SBufferView _bufferView, size_t _offset, size_t _bufferSize) noexcept
		: SBufferView(_bufferView.GetBuffer(), _offset, _bufferSize)
	{
	}

	void RemoveFromFront(size_t _count) noexcept { mBuffer = (char*)mBuffer + _count; }
	void RemoveFromBack(size_t _count) noexcept { mBufferSize -= _count; }

	const void* GetBuffer() const noexcept { return mBuffer; }
	size_t GetBufferSize() const noexcept { return mBufferSize; }
	bool IsEmpty() const noexcept { return mBufferSize == 0; }

private:
	const void* mBuffer = nullptr;
	size_t mBufferSize = 0;
};

class SRange
{
public:
	SRange() noexcept = default;
	SRange(const SRange&) noexcept = default;
	SRange(SRange&&) noexcept = default;

	SRange(size_t _rangeSize) noexcept
		: mRangeSize(_rangeSize)
	{
	}

	SRange(size_t _offset, size_t _rangeSize) noexcept
		: mBegin(_offset), mRangeSize(_rangeSize)
	{
	}

	SRange(SRange _range, size_t _bufferSize) noexcept
		: SRange(_range.GetBegin(), _bufferSize)
	{
	}

	SRange(SRange _range, size_t _offset, size_t _bufferSize) noexcept
		: SRange(_range.GetBegin() + _offset, _bufferSize)
	{
	}

	void RemoveFromFront(size_t _count) noexcept { mBegin += _count; }
	void RemoveFromBack(size_t _count) noexcept { mRangeSize -= _count; }

	size_t GetBegin() const noexcept { return mBegin; }
	const void* GetBuffer(const void* _buffer) const noexcept { return static_cast<const uint8_t*>(_buffer) + mBegin; }
	const void* GetBuffer(const SBlob& _blob) const noexcept { return GetBuffer(_blob.GetBuffer()); }
	void* GetBuffer(void* _buffer) const noexcept { return static_cast<uint8_t*>(_buffer) + mBegin; }
	void* GetBuffer(SBlob& _blob) const noexcept { return GetBuffer(_blob.GetBuffer()); }

	size_t GetSize() const noexcept { return mRangeSize; }
	bool IsEmpty() const noexcept { return mRangeSize == 0; }

private:
	size_t mBegin = 0;
	size_t mRangeSize = 0;
};