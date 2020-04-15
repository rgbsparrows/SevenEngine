#include "EngineMain.h"
#include "EngineLoop.h"

void SEngineMain::EngineMain() noexcept
{
	SEngineLoop::Get().PreInit();
	SEngineLoop::Get().Init();
	SEngineLoop::Get().Run();
	SEngineLoop::Get().Clear();
}

void SEngineMain::RequireExit() noexcept
{
	SEngineLoop::Get().RequireExit();
}
