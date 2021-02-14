#include "EngineLoop.h"
#include "Core/Thread.h"
#include "Core/Class/ClassManager.h"
#include "Core/Modules/ModuleManager.h"

void SEngineLoop::PreInit() noexcept
{
	Thread::SetGameThreadId(std::this_thread::get_id());

	SModuleManager::Get().Init();
	SClassManager::Get().Init();

	SModuleManager::Get().LoadModule(L"UIModule");
	SModuleManager::Get().LoadModule(L"RDIModule");
}

void SEngineLoop::Run() noexcept
{
	while (!mShouldExit)
	{
	}
}
