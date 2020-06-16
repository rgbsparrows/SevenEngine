#pragma once

#include <vcruntime_new.h>

class SRawAllocator
{
public:
	void* Allocate(size_t _size, size_t _align)
	{
		return operator new(_size, static_cast<std::align_val_t>(_align), std::nothrow);
	}

	void Deallocate(void* _buffer, size_t _align)
	{
		operator delete(_buffer, static_cast<std::align_val_t>(_align), std::nothrow);
	}
};
