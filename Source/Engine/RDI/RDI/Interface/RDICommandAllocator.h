#pragma once

__interface IRDICommandAllocator
{
	void Reset() noexcept;
	void Release() noexcept;
};
