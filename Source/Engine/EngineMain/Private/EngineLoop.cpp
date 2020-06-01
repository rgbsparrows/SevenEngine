#include "Thread.h"
#include "EngineLoop.h"
#include "Class/ClassManager.h"
#include "Modules/ModuleManager.h"

void SEngineLoop::PreInit() noexcept
{
	Thread::SetGameThreadId(std::this_thread::get_id());

	SModuleManager::Get().Init();
	SClassManager::Get().Init();
}
