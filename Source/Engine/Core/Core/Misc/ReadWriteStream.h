#pragma once

#include "Core/Math/Math.h"
#include "Core/Util/Assert.h"
#include "Core/Container/Blob.h"

#include <string>
#include <filesystem>

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
		CHECK(mCurrentPos + _size < mBufferView.GetBufferSize());

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
		TSerialize<_type, _serializeFlags...>().Deserialize(*this, _value);
	}

	template<typename _type, typename... _serializeFlags>
	_type Read() noexcept
	{
		_type value;
		TSerialize<_type, _serializeFlags...>().Deserialize(*this, value);
		return value;
	}

	SReadStream MakeSubReadStream(size_t _size) noexcept
	{
		CHECK(_size + mCurrentPos < mBufferView.GetBufferSize());
		return SReadStream(SBufferView(mBufferView, mCurrentPos, _size));
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

	void Seek(size_t _pos = SIZE_MAX) noexcept
	{
		mCurrentPos = std::min(_pos, mContentSize);
	}

	size_t GetContentSize() const noexcept
	{
		return mContentSize;
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

struct SReadStreamStat
{
	SReadStreamStat() noexcept = delete;
	SReadStreamStat(const SReadStreamStat&) noexcept = delete;
	SReadStreamStat(SReadStreamStat&&) noexcept = delete;

	SReadStreamStat(size_t& _writeByteCount, SReadStream& _readStream) noexcept
		:mReadByteCount(_writeByteCount), mReadStream(_readStream)
	{
		mBeginPos = mReadStream.GetCurrentPos();
	}

	~SReadStreamStat() noexcept
	{
		mReadByteCount = mReadStream.GetCurrentPos() - mBeginPos;
	}

	size_t& mReadByteCount;
	SReadStream& mReadStream;
	size_t mBeginPos = 0;
};

struct SWriteStreamStat
{
	SWriteStreamStat(size_t& _writeByteCount, SWriteStream& _writeStream) noexcept
		:mWriteByteCount(_writeByteCount), mWriteStream(_writeStream)
	{
		mBeginPos = mWriteStream.GetCurrentPos();
	}

	~SWriteStreamStat() noexcept
	{
		mWriteByteCount = mWriteStream.GetCurrentPos() - mBeginPos;
	}

	size_t& mWriteByteCount;
	SWriteStream& mWriteStream;
	size_t mBeginPos = 0;
};

//允许类型重载时仅实现Serialize或Deserialize，或是二者均不实现，屏蔽序列化功能
template<typename _type, typename... _serializeFlags>
struct TSerialize
{
	using ValueType = _type;

	void Serialize(SWriteStream& _writeStream, const ValueType& _value) noexcept
	{
		_writeStream.WriteBuffer(SBufferView(&_value, sizeof(ValueType)));
	}

	void Deserialize(SReadStream& _readStream, ValueType& _value) noexcept
	{
		_readStream.ReadBuffer(&_value, sizeof(ValueType));
	}
};

template<>
struct TSerialize<std::string>
{
	using ValueType = std::string;

	void Serialize(SWriteStream& _writeStream, const ValueType& _value) noexcept
	{
		uint16_t strLen = static_cast<uint16_t>(_value.size());
		_writeStream.WriteBuffer(SBufferView(&strLen, sizeof(uint16_t)));
		_writeStream.WriteBuffer(SBufferView(_value.data(), strLen));
	}

	void Deserialize(SReadStream& _readStream, ValueType& _value) noexcept
	{
		uint16_t strLen = _readStream.Read<uint16_t>();
		_value.resize(strLen);
		_readStream.ReadBuffer(&_value[0], strLen);
	}
};

template<>
struct TSerialize<std::string_view>
{
	using ValueType = std::string_view;

	void Serialize(SWriteStream& _writeStream, const ValueType& _value) noexcept
	{
		uint16_t strLen = static_cast<uint16_t>(_value.size());
		_writeStream.WriteBuffer(SBufferView(&strLen, sizeof(uint16_t)));
		_writeStream.WriteBuffer(SBufferView(_value.data(), strLen));
	}
};

template<>
struct TSerialize<std::wstring>
{
	using ValueType = std::wstring;

	void Serialize(SWriteStream& _writeStream, const ValueType& _value) noexcept
	{
		uint16_t strLen = static_cast<uint16_t>(_value.size());
		_writeStream.WriteBuffer(SBufferView(&strLen, sizeof(uint16_t)));
		_writeStream.WriteBuffer(SBufferView(_value.data(), strLen * 2));
	}

	void Deserialize(SReadStream& _readStream, ValueType& _value) noexcept
	{
		uint16_t strLen = _readStream.Read<uint16_t>();
		_value.resize(strLen);
		_readStream.ReadBuffer(&_value[0], strLen * 2);
	}
};

template<>
struct TSerialize<std::wstring_view>
{
	using ValueType = std::wstring_view;

	void Serialize(SWriteStream& _writeStream, const ValueType& _value) noexcept
	{
		uint16_t strLen = static_cast<uint16_t>(_value.size());
		_writeStream.WriteBuffer(SBufferView(&strLen, sizeof(uint16_t)));
		_writeStream.WriteBuffer(SBufferView(_value.data(), strLen * 2));
	}
};

template<>
struct TSerialize<std::filesystem::path>
{
	using ValueType = std::filesystem::path;

	void Serialize(SWriteStream& _writeStream, const ValueType& _value) noexcept
	{
		_writeStream.Write<std::wstring>(_value.native());
	}

	void Deserialize(SReadStream& _readStream, ValueType& _value) noexcept
	{
		std::wstring path = _readStream.Read<std::wstring>();
		_value = path;
	}
};

template<>
struct TSerialize<SBlob>
{
	using ValueType = SBlob;

	void Serialize(SWriteStream& _writeStream, const ValueType& _value) noexcept
	{
		_writeStream.WriteBuffer(_value);
	}

	void Deserialize(SReadStream& _readStream, ValueType& _value) noexcept
	{
		_value.ResizeBlob(_readStream.GetContentSize());
		_readStream.ReadBuffer(_value.GetBuffer(), _value.GetBufferSize());
	}
};

template<>
struct TSerialize<SBufferView>
{
	using ValueType = SBufferView;

	void Serialize(SWriteStream& _writeStream, const ValueType& _value) noexcept
	{
		_writeStream.WriteBuffer(_value);
	}
};

template<typename _type, typename... _serializeFlags>
struct TSerialize<std::vector<_type>, _serializeFlags...>
{
	using ValueType = std::vector<_type>;

	void Serialize(SWriteStream& _writeStream, const ValueType& _value) noexcept
	{
		uint16_t strLen = static_cast<uint16_t>(_value.size());
		_writeStream.WriteBuffer(SBufferView(&strLen, sizeof(uint16_t)));
		for (size_t i = 0; i != _value.size(); ++i)
			_writeStream.Write<_type, _serializeFlags...>(_value[i]);
	}

	void Deserialize(SReadStream& _readStream, ValueType& _value) noexcept
	{
		uint16_t vecSize = _readStream.Read<uint16_t>();
		_value.resize(vecSize);
		for (size_t i = 0; i != _value.size(); ++i)
			_readStream.Read<_type, _serializeFlags...>(_value[i]);
	}
};
