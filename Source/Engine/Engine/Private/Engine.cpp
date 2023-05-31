#include "Engine/World.h"
#include "Engine/Engine.h"

void SEngine::Tick() noexcept
{
	mMainWorld->Tick();
}

void SEngine::ReplaceMainWorld(SWorld* _mainWorld) noexcept
{
	if (mMainWorld)
		mMainWorld->Release();

	mMainWorld = _mainWorld;
}
