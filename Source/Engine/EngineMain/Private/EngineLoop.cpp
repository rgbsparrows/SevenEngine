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
	Thread::SetCurrentThreadName(L"主线程");

	SModuleManager::Get().Init();
	SClassManager::Get().Init();

	SModuleManager::Get().LoadModule(L"RenderModule");
	SModuleManager::Get().LoadModule(L"UIModule");
	SModuleManager::Get().LoadModule(L"EditorModule");
}

void SEngineLoop::Run() noexcept
{
	while (!mShouldExit)
	{
		GClock.Tick();

		GetRenderModule()->BeginFrame_GameThread();
		GetUIModule()->OnGUI();



		GetRenderModule()->EndFrame_GameThread();
	}
}

void SEngineLoop::Clear() noexcept
{
	SModuleManager::Get().UnloadModule(L"EditorModule");
	SModuleManager::Get().UnloadModule(L"UIModule");
	SModuleManager::Get().UnloadModule(L"RenderModule");
}
