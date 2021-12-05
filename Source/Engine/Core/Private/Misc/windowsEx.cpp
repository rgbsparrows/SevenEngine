#include "Core/Misc/windowsEx.h"

#include <thread>

void YieldForSingleObject(HANDLE _handle) noexcept
{
	while (WaitForSingleObject(_handle, INFINITE) == WAIT_TIMEOUT)
		std::this_thread::yield();
}
