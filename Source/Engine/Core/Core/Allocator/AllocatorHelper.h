#pragma once

#include "Core/Math/Operation.h"
#include "Core/Macros/Assert.h"

#include <limits>
#include <vector>
#include <stdint.h>

class SCircularSlotAllocatorHelper
{
public:
	SCircularSlotAllocatorHelper() noexcept = default;
	SCircularSlotAllocatorHelper(uint64_t _slotCount) noexcept
	{
		ResetSlotCount(_slotCount);
	}

	//����Ҫ����slotʱ����Ҫ����
	void ResetSlotCount(uint64_t _slotCount) noexcept
	{
		CHECK((_slotCount & 0x3f) == 0);
		CHECK(Math::CalcBlockCount(_slotCount, 64ull) >= mBitmap.size());

		mBitmap.resize(Math::CalcBlockCount(_slotCount, 64ull));
	}

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>s���������������ֵΪstd::numeric_limits<uint64_t>::max()ʱ��ʾ����ʧ��</returns>
	uint64_t AllocateSlot() noexcept
	{
		uint64_t preWordIndex = mCurrentWord;

		for (void(); mCurrentWord != mBitmap.size(); ++mCurrentWord)
		{
			if (mBitmap[mCurrentWord] == std::numeric_limits<uint64_t>::max()) continue;

			for (uint32_t i = 0; i != 64; ++i)
			{
				if ((mBitmap[mCurrentWord] >> i & 1) == 0)
				{
					mBitmap[mCurrentWord] |= 1ull << i;
					return mCurrentWord * 64 + i;
				}
			}
		}

		for (mCurrentWord = 0; mCurrentWord != preWordIndex; ++mCurrentWord)
		{
			if (mBitmap[mCurrentWord] == std::numeric_limits<uint64_t>::max()) continue;

			for (uint32_t i = 0; i != 32; ++i)
			{
				if ((mBitmap[mCurrentWord] >> i & 1) == 0)
				{
					mBitmap[mCurrentWord] |= 1ull << i;
					return mCurrentWord * 64 + i;
				}
			}
		}

		return std::numeric_limits<uint64_t>::max();
	}

	void DeallocateSlot(uint64_t _slotIndex)
	{
		CHECK(_slotIndex <= mBitmap.size() * 64ull);

		uint64_t wordIndex = _slotIndex / 64;
		uint32_t bitIndex = _slotIndex % 64;

		mBitmap[wordIndex] &= ~(1 << bitIndex);
	}

	uint64_t GetSize() const
	{
		return mBitmap.size() * 64ull;
	}

private:
	std::vector<uint64_t> mBitmap;
	uint64_t mCurrentWord = 0;
};

class SCircularSlotArrayAllocatorHelper
{
public:
	SCircularSlotArrayAllocatorHelper() noexcept = default;
	SCircularSlotArrayAllocatorHelper(uint64_t _slotCount) noexcept
	{
		CHECK((_slotCount & 0x7f) == 0);

		mBitmap.resize(Math::CalcBlockCount(_slotCount, 64ull));
	}

	void ResetSlotCount(uint64_t _slotCount) noexcept
	{
		CHECK((_slotCount & 0x7f) == 0);
		CHECK(Math::CalcBlockCount(_slotCount, 64ull) >= mBitmap.size());

		mBitmap.resize(Math::CalcBlockCount(_slotCount, 64ull));
	}

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>s���������������ֵΪuint64_t::max()ʱ��ʾ����ʧ��</returns>
	uint64_t AllocateSlotArray(uint64_t _arraySize) noexcept
	{
		CHECK(_arraySize <= 64ull);
		CHECK(_arraySize != 0ull);

		uint64_t slotMask[63][2];

		if (_arraySize == 64ull)
			slotMask[0][0] = std::numeric_limits<uint64_t>::max();
		else
			slotMask[0][0] = (1ull << _arraySize) - 1ull;

		slotMask[0][1] = 0;

		for (size_t i = 1; i != 63; ++i)
		{
			slotMask[i][0] = slotMask[0][0] << i;
			slotMask[i][1] = slotMask[0][0] >> (64 - i);
		}

		uint64_t preWordIndex = mCurrentWord;

		for (void(); mCurrentWord + 1 < mBitmap.size(); ++mCurrentWord)
		{
			for (uint32_t i = 0; i != 63; ++i)
			{
				if (
					(mBitmap[mCurrentWord] & slotMask[i][0]) == slotMask[i][0] &&
					(mBitmap[mCurrentWord + 1] & slotMask[i][1]) == slotMask[i][1]
					)
				{
					mBitmap[mCurrentWord] &= slotMask[i][0];
					mBitmap[mCurrentWord + 1] &= slotMask[i][1];
					return mCurrentWord * 64 + i;
				}
			}
		}

		++mCurrentWord;
		for (uint32_t i = 0; i != 64 - _arraySize + 1; ++i)
		{
			if ((mBitmap[mCurrentWord] & slotMask[i][0]) == slotMask[i][0])
			{
				mBitmap[mCurrentWord] &= slotMask[i][0];
				return mCurrentWord * 64 + i;
			}
		}

		for (mCurrentWord = 0; mCurrentWord != preWordIndex; ++mCurrentWord)
		{
			for (uint32_t i = 0; i != 63; ++i)
			{
				if (
					(mBitmap[mCurrentWord] & slotMask[i][0]) == slotMask[i][0] &&
					(mBitmap[mCurrentWord + 1] & slotMask[i][1]) == slotMask[i][1]
					)
				{
					mBitmap[mCurrentWord] &= slotMask[i][0];
					mBitmap[mCurrentWord + 1] &= slotMask[i][1];
					return mCurrentWord * 64 + i;
				}
			}
		}

		return std::numeric_limits<uint64_t>::max();
	}

	void DeallocateSlotArray(uint64_t _slotIndex, uint64_t _arraySize)
	{
		uint64_t wordIndex = _slotIndex / 64;
		uint32_t bitIndex = _slotIndex % 64;

		uint64_t slotMask[2];
		if(_arraySize == 64ull)
			slotMask[0] = std::numeric_limits<uint64_t>::max();
		else
			slotMask[0] = (1ull << _arraySize) - 1ull;

		slotMask[1] = slotMask[0] >> (64 - bitIndex);
		slotMask[0] = slotMask[0] << bitIndex;

		mBitmap[wordIndex] &= ~slotMask[0];
		
		if(wordIndex + 1 != mBitmap.size())
			mBitmap[wordIndex + 1] &= ~slotMask[1];
	}

	uint64_t GetSize() const
	{
		return mBitmap.size() * 64ull;
	}

private:
	std::vector<uint64_t> mBitmap;
	uint64_t mCurrentWord = 0;
};