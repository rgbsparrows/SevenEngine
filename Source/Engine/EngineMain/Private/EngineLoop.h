#pragma once

#include "Core/Util/TemplateUtil.h"

class SEngineLoop final : public TAsSingleton<SEngineLoop>
{
public:
	void PreInit() noexcept;
	void Init() noexcept;
	void Run() noexcept;
	void Clear() noexcept;

	void RequireExit() noexcept { mShouldExit = true; }

private:
	bool mShouldExit = false;
};
