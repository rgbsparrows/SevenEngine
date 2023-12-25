#include "Engine/World.h"
#include "Engine/Engine.h"


void SEngine::Init() noexcept
{
	TODO("后续可以替换为其他默认场景");
	ReplaceMainWorld(SWorld::MakeNewWorld());
}

void SEngine::Tick() noexcept
{
	mMainWorld->Tick();
}

void SEngine::Clear() noexcept
{
	if (mMainWorld)
		mMainWorld->Release();
}

void SEngine::ReplaceMainWorld(SWorld* _mainWorld) noexcept
{
	if (mMainWorld)
		mMainWorld->Release();

	mMainWorld = _mainWorld;
}
