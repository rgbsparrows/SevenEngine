#pragma once

#include "Core/Modules/ModuleInterface.h"

__interface IRenderModule : public IModuleInterface
{
	bool Init() noexcept;
	void Clear() noexcept;

	void BeginFrame_GameThread() noexcept;
	void EndFrame_GameThread() noexcept;
};

IRenderModule* GetRenderModule() noexcept;
