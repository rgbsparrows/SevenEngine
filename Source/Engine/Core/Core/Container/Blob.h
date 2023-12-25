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

template <bool _isConstBuffer>
class TBufferViewBase
{
public:
	static constexpr bool IsConstBuffer = _isConstBuffer;

	using UnderlyingType = std::conditional_t<IsConstBuffer, const void*, void*>;
	using UnderlyingUint8Type = std::conditional_t<IsConstBuffer, const uint8_t*, uint8_t*>;
	using BlobRefType = std::conditional_t<IsConstBuffer, const SBlob&, SBlob&>;

	TBufferViewBase() noexcept = default;
	TBufferViewBase(const TBufferViewBase&) noexcept = default;

	TBufferViewBase(UnderlyingType _buffer, size_t _bufferSize) noexcept
		: mBuffer(_buffer), mBufferSize(_bufferSize)
	{
	}

	TBufferViewBase(UnderlyingType _buffer, size_t _offset, size_t _bufferSize) noexcept
		: mBuffer(static_cast<UnderlyingUint8Type>(_buffer) + _offset), mBufferSize(_bufferSize)
	{
	}

	TBufferViewBase(BlobRefType _blob) noexcept
		: TBufferViewBase(_blob.GetBuffer(), _blob.GetBufferSize())
	{
	}

	TBufferViewBase(BlobRefType _blob, size_t _offset) noexcept
		: TBufferViewBase(_blob.GetBuffer(), _offset, _blob.GetBufferSize() - _offset)
	{
	}

	TBufferViewBase(BlobRefType _blob, size_t _offset, size_t _bufferSize) noexcept
		: TBufferViewBase(_blob.GetBuffer(), _offset, _bufferSize)
	{
	}

	TBufferViewBase(TBufferViewBase _bufferView, size_t _bufferSize) noexcept
		: TBufferViewBase(_bufferView.GetBuffer(), _bufferSize)
	{
	}

	TBufferViewBase(TBufferViewBase _bufferView, size_t _offset, size_t _bufferSize) noexcept
		: TBufferViewBase(_bufferView.GetBuffer(), _offset, _bufferSize)
	{
	}

	void RemoveFromFront(size_t _count) noexcept { mBuffer = (char*)mBuffer + _count; }
	void RemoveFromBack(size_t _count) noexcept { mBufferSize -= _count; }

	const UnderlyingType GetBuffer() const noexcept { return mBuffer; }
	UnderlyingType GetBuffer() noexcept { return mBuffer; }

	size_t GetBufferSize() const noexcept { return mBufferSize; }
	bool IsEmpty() const noexcept { return mBufferSize == 0; }

private:
	UnderlyingType mBuffer = nullptr;
	size_t mBufferSize = 0;
};

using SConstBufferView = TBufferViewBase<true>;
using SBufferView = TBufferViewBase<false>;

class SRange
{
public:
	SRange() noexcept = default;
	SRange(const SRange&) noexcept = default;

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
	SConstBufferView GetBuffer(const void* _buffer) const noexcept { return SConstBufferView(_buffer, mBegin, mRangeSize); }
	SConstBufferView GetBuffer(const SBlob& _blob) const noexcept { return SConstBufferView(_blob, mBegin, mRangeSize); }
	SBufferView GetBuffer(void* _buffer) const noexcept { return SBufferView(_buffer, mBegin, mRangeSize); }
	SBufferView GetBuffer(SBlob& _blob) const noexcept { return SBufferView(_blob, mBegin, mRangeSize); }

	size_t GetSize() const noexcept { return mRangeSize; }
	bool IsEmpty() const noexcept { return mRangeSize == 0; }

private:
	size_t mBegin = 0;
	size_t mRangeSize = 0;
};

inline void Memcpy(SBufferView _buffer, const void* _src, size_t _size) noexcept
{
	memcpy_s(_buffer.GetBuffer(), _buffer.GetBufferSize(), _src, _size);
}

inline void Memcpy(SBufferView _buffer, SBufferView _data) noexcept
{
	Memcpy(_buffer, _data.GetBuffer(), _data.GetBufferSize());
}

inline void Memcpy(SBufferView _buffer, SConstBufferView _data) noexcept
{
	Memcpy(_buffer, _data.GetBuffer(), _data.GetBufferSize());
}
