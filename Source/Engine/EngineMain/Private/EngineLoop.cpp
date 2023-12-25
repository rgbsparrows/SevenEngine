#include "EngineLoop.h"
#include "UI/UIModule.h"
#include "Engine/Engine.h"
#include "Core/Clock/Clock.h"
#include "Core/Misc/Thread.h"
#include "Render/RenderModule.h"
#include "Core/Class/ClassManager.h"
#include "Core/Modules/ModuleManager.h"

void SEngineLoop::PreInit() noexcept
{
	srand(static_cast<uint32_t>(time(nullptr)));
	Thread::SetGameThreadId(std::this_thread::get_id());
	Thread::SetCurrentThreadName(u8"主线程");

	SModuleManager::Get().Init();
	SClassManager::Get().Init();

	SModuleManager::Get().LoadModule("ResourceModule");
	SModuleManager::Get().LoadModule("RenderModule");
}

void SEngineLoop::Init() noexcept
{
	GetRenderModule()->BeginFrame_GameThread();
	SModuleManager::Get().LoadAllEnableModule();
	SEngine::Get().Init();
	GetRenderModule()->EndFrame_GameThread();
}

void SEngineLoop::Run() noexcept
{
	while (!mShouldExit)
	{
		SClock::Get().Tick();

		GetRenderModule()->BeginFrame_GameThread();
		SEngine::Get().Tick();
		GetUIModule()->OnGUI();
		GetRenderModule()->EndFrame_GameThread();
	}
}

void SEngineLoop::Clear() noexcept
{
	GetRenderModule()->BeginFrame_GameThread();
	SEngine::Get().Clear();
	SModuleManager::Get().UnloadAllEnableModule();
	GetRenderModule()->EndFrame_GameThread();

	SModuleManager::Get().UnloadModule("RenderModule");
	SModuleManager::Get().UnloadModule("ResourceModule");
}
