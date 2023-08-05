#include "EngineLoop.h"
#include "Core/Misc/Thread.h"
#include "Render/RenderModule.h"
#include "Core/Class/ClassManager.h"
#include "Core/Modules/ModuleManager.h"
#include "Core/Clock/Clock.h"
#include "UI/UIModule.h"

void SEngineLoop::PreInit() noexcept
{
	Thread::SetGameThreadId(std::this_thread::get_id());
	Thread::SetCurrentThreadName(u8"主线程");

	SModuleManager::Get().Init();
	SClassManager::Get().Init();

	SModuleManager::Get().LoadModule("ResourceModule");
	SModuleManager::Get().LoadModule("RenderModule");
	SModuleManager::Get().LoadModule("UIModule");
	SModuleManager::Get().LoadModule("EditorModule");
}

void SEngineLoop::Run() noexcept
{
	while (!mShouldExit)
	{
		SClock::Get().Tick();

		GetRenderModule()->BeginFrame_GameThread();
		GetUIModule()->OnGUI();



		GetRenderModule()->EndFrame_GameThread();
	}
}

void SEngineLoop::Clear() noexcept
{
	SModuleManager::Get().UnloadModule("EditorModule");
	SModuleManager::Get().UnloadModule("UIModule");
	SModuleManager::Get().UnloadModule("RenderModule");
	SModuleManager::Get().UnloadModule("ResourceModule");
}
