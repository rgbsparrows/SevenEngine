#pragma once

#include "Core/Math/Operation.h"
#include "Core/Allocator/AllocatorHelper.h"

#include <limits>
#include <vector>
#include <stdint.h>
#include <vcruntime_new.h>

class SRawAllocator
{
public:
	void* Allocate(size_t _size, size_t _align) noexcept
	{
		return operator new(_size, static_cast<std::align_val_t>(_align), std::nothrow);
	}

	void Deallocate(void* _buffer, size_t _align) noexcept
	{
		operator delete(_buffer, static_cast<std::align_val_t>(_align), std::nothrow);
	}
};
