#include "EngineLoop.h"
#include "Core/Thread.h"
#include "Render/RenderModule.h"
#include "Core/Class/ClassManager.h"
#include "Core/Modules/ModuleManager.h"
#include "Core/fmt/format.h"

void SEngineLoop::PreInit() noexcept
{
	Thread::SetGameThreadId(std::this_thread::get_id());

	SModuleManager::Get().Init();
	SClassManager::Get().Init();

	SModuleManager::Get().LoadModule(L"RenderModule");

}

void SEngineLoop::Run() noexcept
{
	while (!mShouldExit)
	{
		GetRenderModule()->BeginFrame_GameThread();

		GetRenderModule()->EndFrame_GameThread();
	}
}

void SEngineLoop::Clear() noexcept
{
	SModuleManager::Get().UnloadModule(L"RenderModule");
}
