#pragma once

#include "Core/Modules/ModuleInterface.h"

constexpr size_t GRenderInfoCount = 3;

__interface IRenderModule : IModuleInterface
{
public:
	bool Init() noexcept;
	void Clear() noexcept;

	void BeginFrame_GameThread() noexcept;
	void EndFrame_GameThread() noexcept;

	size_t GetFrameInfoIndex_GameThread() noexcept;
	size_t GetFrameInfoIndex_RenderThread() noexcept;
};

IRenderModule* GetRenderModule() noexcept;
