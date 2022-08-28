#pragma once

#include "Core/Math/Math.h"
#include "Core/Container/Blob.h"
#include <string>

class SReadStream
{
public:
	SReadStream() noexcept = default;
	SReadStream(const SReadStream&) noexcept = default;
	SReadStream(SReadStream&& _readSteam) noexcept
	{
		mBufferView = _readSteam.mBufferView;
		mCurrentPos = _readSteam.mCurrentPos;
		_readSteam.mCurrentPos = 0;
	}

	SReadStream(SBufferView _bufferView) noexcept
		:mBufferView(_bufferView)
	{
	}

	size_t GetCurrentPos() const noexcept
	{
		return mCurrentPos;
	}

	size_t Seek(size_t _pos = SIZE_MAX) noexcept
	{
		mCurrentPos = std::min(_pos, mBufferView.GetBufferSize());
	}

	size_t GetContentSize() const noexcept
	{
		return mBufferView.GetBufferSize();
	}

	bool IsReachedEnd() const noexcept
	{
		return mCurrentPos == GetContentSize();
	}

	SBufferView ReadBuffer(size_t _size) noexcept
	{
		if (mCurrentPos + _size >= mBufferView.GetBufferSize())
			return SBufferView();

		SBufferView buffer = SBufferView(mBufferView, mCurrentPos, _size);
		mCurrentPos += _size;

		return buffer;
	}

	void ReadBuffer(void* _buffer, size_t _size) noexcept
	{
		SBufferView buffer = ReadBuffer(_size);
		memcpy_s(_buffer, _size, buffer.GetBuffer(), buffer.GetBufferSize());
	}

	template<typename _type, typename... _serializeFlags>
	void Read(_type& _value) noexcept
	{
		TSerialize<_type, _serializeFlags...>().Deserialize(SBufferView(mBufferView, mCurrentPos), _value);
	}

	template<typename _type, typename... _serializeFlags>
	_type Read() noexcept
	{
		_type value;
		TSerialize<_type, _serializeFlags...>().Deserialize(SBufferView(mBufferView, mCurrentPos), value);
		return value;
	}

private:
	SBufferView mBufferView;
	size_t mCurrentPos = 0;
};

class SWriteStream
{
public:
	SWriteStream() noexcept = default;
	SWriteStream(const SWriteStream&) noexcept = default;
	SWriteStream(SWriteStream&& _writeStream) noexcept
	{
		mBlob = std::move(_writeStream.mBlob);
		mContentSize = _writeStream.mContentSize;
		_writeStream.mCurrentPos = 0;
		_writeStream.mContentSize = 0;
	}

	size_t GetCurrentPos() const noexcept
	{
		return mCurrentPos;
	}

	size_t Seek(size_t _pos = SIZE_MAX) noexcept
	{
		mCurrentPos = std::min(_pos, mContentSize);
	}

	SBufferView GetContentBuffer() const noexcept
	{
		return SBufferView(mBlob, 0, mContentSize);
	}

	const void WriteBuffer(SBufferView _buffer) noexcept
	{
		EnsureRestBlobSize(_buffer.GetBufferSize());

		void* dest = reinterpret_cast<uint8_t*>(mBlob.GetBuffer()) + mCurrentPos;
		memcpy_s(dest, _buffer.GetBufferSize(), _buffer.GetBuffer(), _buffer.GetBufferSize());
		mCurrentPos += _buffer.GetBufferSize();
		mContentSize = std::max(mCurrentPos, mContentSize);
	}

	template<typename _type, typename... _serializeFlags>
	void Write(const _type& _value) noexcept
	{
		TSerialize<_type, _serializeFlags...>().Serialize(*this, _value);
	}

private:
	void EnsureRestBlobSize(size_t _size) noexcept
	{
		if (_size + mCurrentPos < mBlob.GetBufferSize())
			return;

		SBufferView buffer = mBlob;
		SBlob newBlob = SBlob(Math::CalcBlockCount<size_t>(_size + mContentSize + mContentSize / 2, 4096) * 4096);
		memcpy_s(newBlob.GetBuffer(), newBlob.GetBufferSize(), mBlob.GetBuffer(), mContentSize);
		mBlob = std::move(newBlob);
	}

	SBlob mBlob = SBlob(4096);
	size_t mCurrentPos = 0;
	size_t mContentSize = 0;
};

template<typename _type, typename... _serializeFlags>
struct TSerialize
{
	using SValueType = _type;

	void Serialize(SWriteStream& _writeStream, const SValueType& _value) noexcept
	{
		_writeStream.WriteBuffer(SBufferView(&_value, sizeof(SValueType)));
	}

	void Deserialize(SReadStream& _readStream, SValueType& _value) noexcept
	{
		SBufferView buffer = _readStream.ReadBuffer();
		memcpy_s(&_value, sizeof(SValueType), _bufferView.GetBuffer(), _bufferView.GetBufferSize());
	}
};

template<>
struct TSerialize<std::string>
{
	using SValueType = std::string;

	void Serialize(SWriteStream& _writeStream, const SValueType& _value) noexcept
	{
		uint16_t strLen = static_cast<uint16_t>(_value.size());
		_writeStream.WriteBuffer(SBufferView(&strLen, sizeof(uint16_t)));
		_writeStream.WriteBuffer(SBufferView(_value.data(), strLen));
	}

	void Deserialize(SReadStream& _readStream, SValueType& _value) noexcept
	{
		uint16_t strLen = _readStream.Read<uint16_t>();
		_value.resize(strLen);
		_readStream.ReadBuffer(&_value[0], strLen);
	}
};

template<typename _type, typename... _serializeFlags>
struct TSerialize<std::vector<_type>, _serializeFlags...>
{
	using SValueType = std::vector<_type>;

	void Serialize(SWriteStream& _writeStream, const SValueType& _value) noexcept
	{
		uint16_t strLen = static_cast<uint16_t>(_value.size());
		_writeStream.WriteBuffer(SBufferView(&strLen, sizeof(uint16_t)));
		for (size_t i = 0; i != _value.size(); ++i)
			_writeStream.Write<_type, _serializeFlags...>(_value[i]);
	}

	void Deserialize(SReadStream& _readStream, SValueType& _value) noexcept
	{
		uint16_t vecSize = _readStream.Read<uint16_t>();
		_value.resize(vecSize);
		for (size_t i = 0; i != _value.size(); ++i)
			_readStream.Read<_type, _serializeFlags...>(_value[i]);
	}
};
