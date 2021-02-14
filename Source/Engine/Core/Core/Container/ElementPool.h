#pragma once

#include "Core/Allocator/AllocatorHelper.h"
#include "Core/Template/TemplateUtil.h"

template<typename _elementType>
class TElementPool
{
public:
	using ElementType = _elementType;
	static constexpr size_t ElementSize = sizeof(ElementType);
	static constexpr size_t ElementAligned = alignof(ElementType);
	using UnconstructElement = TUnconstructAlignedElement<ElementSize, ElementAligned>;

	TElementPool() = default;
	TElementPool(uint64_t _elementCount)
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
