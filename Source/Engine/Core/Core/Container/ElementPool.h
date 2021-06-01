#pragma once

#include "Core/Macros/UtilMacros.h"
#include "Core/Template/TemplateUtil.h"
#include "Core/Allocator/AllocatorHelper.h"

#include <mutex>

namespace detail
{
	template<typename _elementType>
	class TElementPoolInternal
	{
	public:
		using ElementType = _elementType;
		static constexpr size_t ElementSize = sizeof(ElementType);
		static constexpr size_t ElementAligned = alignof(ElementType);
		using UnconstructElement = TUnconstructAlignedElement<ElementSize, ElementAligned>;

		TElementPoolInternal() = default;
		TElementPoolInternal(uint64_t _elementCount)
		{
			CHECK((_elementCount & 0x3f) == 0);

			mUnconstructElements.resize(_elementCount);
			mAllocatorHelper.ResetSlotCount(_elementCount);
		}

		template<typename... _argt>
		ElementType* AllocateElement(_argt&&... _args) noexcept
		{
			uint64_t index = mAllocatorHelper.AllocateSlot();
			if (index == std::numeric_limits<uint64_t>::max())
				return nullptr;

			return new(&mUnconstructElements[index]) ElementType(std::forward<_argt>(_args)...);
		}

		void DeallocateElement(ElementType* _element)
		{
			CHECK(_element != nullptr);

			uint64_t index = (PointerToInt(_element) - PointerToInt(&mUnconstructElements[0])) / ElementSize;

			_element->~ElementType();
			mAllocatorHelper.DeallocateSlot(index);
		}

		void ResizePool(uint64_t _elementCount)
		{
			CHECK(_elementCount >= mAllocatorHelper.GetSize());

			mUnconstructElements.resize(_elementCount);
			mAllocatorHelper.ResetSlotCount(_elementCount);
		}

	private:
		std::vector<UnconstructElement> mUnconstructElements;
		SCircularSlotAllocatorHelper mAllocatorHelper;
	};
}

enum class EElementPoolFlag
{
	None = 0,
	ThreadSafe = 1 << 0,
};
REGIST_ENUM_FLAG(EElementPoolFlag)

template<typename _elementType, EElementPoolFlag _elementPoolFlag = EElementPoolFlag::None>
class TElementPool {};

template<typename _elementType>
class TElementPool<_elementType, EElementPoolFlag::None> : public detail::TElementPoolInternal<_elementType> {};

template<typename _elementType>
class TElementPool<_elementType, EElementPoolFlag::ThreadSafe>
{
public:
	using TElementPoolInternalType = detail::TElementPoolInternal<_elementType>;
	using ElementType = TElementPoolInternalType::ElementType;
	static constexpr size_t ElementSize = TElementPoolInternalType::ElementSize;
	static constexpr size_t ElementAligned = TElementPoolInternalType::ElementAligned;

	TElementPool() = default;
	TElementPool(uint64_t _elementCount)
		:mElementPoolInternal(_elementCount)
	{
	}

	template<typename... _argt>
	ElementType* AllocateElement(_argt&&... _args) noexcept
	{
		std::lock_guard lock(mCriticalSection);

		mElementPoolInternal.AllocateElement(std::forward<_argt>(_args)...);
	}

	void DeallocateElement(ElementType* _element)
	{
		std::lock_guard lock(mCriticalSection);

		mElementPoolInternal.DeallocateElement(_element);
	}

	void ResizePool(uint64_t _elementCount)
	{
		std::lock_guard lock(mCriticalSection);

		mElementPoolInternal.ResizePool(_elementCount);
	}

private:
	TElementPoolInternalType mElementPoolInternal;
	SCriticalSection mCriticalSection;
};