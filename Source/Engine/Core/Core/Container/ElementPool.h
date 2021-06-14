#pragma once

#include "Core/Util/UtilMacros.h"
#include "Core/Util/TemplateUtil.h"
#include "Core/Container/AllocatorHelper.h"
#include "Core/Misc/Thread.h"

#include <mutex>
enum class EElementPoolFlag
{
	None = 0,
	StaticCapacity = 1 << 0,
	ThreadSafe = 1 << 1,
};
REGIST_ENUM_FLAG(EElementPoolFlag)

template<typename _elementType, EElementPoolFlag _elementPoolFlag = EElementPoolFlag::None>
class TElementPool
{
public:
	using ElementType = _elementType;
	static constexpr size_t ElementSize = sizeof(ElementType);
	static constexpr size_t ElementAligned = alignof(ElementType);
	static constexpr EElementPoolFlag ElementPoolFlag = _elementPoolFlag;
	static constexpr bool IsStaticCapacity = (ElementPoolFlag & EElementPoolFlag::StaticCapacity) == EElementPoolFlag::StaticCapacity;
	static constexpr bool IsThreadSafe = (ElementPoolFlag & EElementPoolFlag::ThreadSafe) == EElementPoolFlag::ThreadSafe;
	using UnconstructElement = TUnconstructAlignedElement<ElementSize, ElementAligned>;
	using LockType = TTypeSwitch<IsThreadSafe, Thread::SCriticalSection, Thread::SDummyLock>;
	using AdditionElementPoolType = TElementPool<ElementType, EElementPoolFlag::None>;
	using AdditionElementPoolListType = TTypeSwitch<IsStaticCapacity, EmptyType, std::vector<AdditionElementPoolType>>;

	TElementPool() noexcept = default;
	TElementPool(uint64_t _newcapacity) noexcept
	{
		CHECK(_newcapacity % 64ull == 0);

		RecapacityPool(_newcapacity);
	}

	template<typename... _argt>
	ElementType* AllocateElement(_argt&&... _args) noexcept
	{
		std::lock_guard lockGuard(mLock);
		CHECK(IsStaticCapacity && IsFull());

		if (IsFull())
			RecapacityPool(GetCapacity() + 1);

		if (mAllocatorHelper.IsFull() == false)
		{
			uint64_t index = mAllocatorHelper.AllocateSlot();
			return new(&mUnconstructElements[index]) ElementType(std::forward<_argt>(_args)...);
		}
		else if constexpr (IsStaticCapacity == false)
		{
			for (auto& _pool : mAdditionElementPool)
			{
				if (_pool.IsFull() == false)
					return _pool.AllocateElement(std::forward<_argt>((_args))...);
			}
		}

		return nullptr;
	}

	void DeallocateElement(ElementType* _element) noexcept
	{
		std::lock_guard lockGuard(mLock);

		CHECK(_element != nullptr);
		CHECK(IsAllocatedFrom(_element));

		uint64_t index = (PointerToInt(_element) - PointerToInt(&mUnconstructElements[0])) / ElementSize;
		if (index >= 0 && index <= mAllocatorHelper.GetSize())
		{
			_element->~ElementType();
			mAllocatorHelper.DeallocateSlot(index);
		}
		else if constexpr (IsStaticCapacity == false)
		{
			for (auto& _pool : mAdditionElementPool)
				_pool.DeallocateElement(_element);
		}
	}

	void RecapacityPool(uint64_t _newcapacity) noexcept
	{
		std::lock_guard lock(mLock);

		CHECK(!IsStaticCapacity || GetCapacity() == 0);
		CHECK(GetCapacity() < _newcapacity);

		if (GetCapacity() == 0)
		{
			mUnconstructElements.resize(_newcapacity);
			mAllocatorHelper.ResetSlotCount(_newcapacity);
		}
		else if constexpr (IsStaticCapacity == false)
		{
			uint64_t newCapacity = Math::CalcBlockCount(std::max(GetCapacity() / 2, _newcapacity - GetCapacity()), 64ull);
			mAdditionElementPool.push_back(TElementPool(newCapacity));
		}
	}

	uint64_t GetCapacity() const noexcept
	{
		std::lock_guard lock(mLock);

		if constexpr (IsStaticCapacity)
			return mAllocatorHelper.GetSize();
		else
		{
			uint64_t capactity = mAllocatorHelper.GetSize();

			for (const auto& _pool : mAdditionElementPool)
				capactity += _pool.GetCapacity();

			return capactity;
		}
	}

	bool IsFull() const noexcept
	{
		std::lock_guard lock(mLock);

		if constexpr (IsStaticCapacity)
			return mAllocatorHelper.IsFull();
		else
		{
			bool isFull = mAllocatorHelper.IsFull();

			for (const auto& _pool : mAdditionElementPool)
				isFull = isFull && _pool.IsFull();

			return isFull;
		}
	}

	bool IsAllocatedFrom(const ElementType* _element) const noexcept
	{
		std::lock_guard lock(mLock);

		if (mAllocatorHelper.GetSize() != 0)
		{
			uint64_t index = (PointerToInt(_element) - PointerToInt(&mUnconstructElements[0])) / ElementSize;
			if (index >= 0 && index <= mAllocatorHelper.GetSize())
				return true;
		}

		if constexpr (IsStaticCapacity == false)
		{
			for (const auto& _pool : mAdditionElementPool)
			{
				if (_pool.IsAllocatedFrom(_element))
					return true;
			}
		}

		return false;
	}

private:
	std::vector<UnconstructElement> mUnconstructElements;
	SCircularSlotAllocatorHelper mAllocatorHelper;
	AdditionElementPoolListType mAdditionElementPool;

	mutable LockType mLock;
};
