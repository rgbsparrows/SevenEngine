#include "Core/Misc/Thread.h"
#include "Core/Misc/windowsEx.h"

#include <thread>

void YieldForSingleObject(HANDLE _handle) noexcept
{
	Thread::YieldWhile(
		[&]() {return WaitForSingleObject(_handle, 0) == WAIT_TIMEOUT; }
	);
}
