#pragma once

#include "Core/Util/TemplateUtil.h"

class SWorld;

class SEngine : public TAsSingleton<SEngine>
{
public:
	void Init() noexcept;
	void Tick() noexcept;
	void Clear() noexcept;

	void ReplaceMainWorld(SWorld* _mainWorld) noexcept;

	SWorld* GetMainWorld() noexcept { return mMainWorld; }

private:
	SWorld* mMainWorld = nullptr;
};
