#include "EngineLoop.h"
#include "Thread.h"

void SEngineLoop::PreInit() noexcept
{
	Thread::SetGameThreadId(std::this_thread::get_id());

}
