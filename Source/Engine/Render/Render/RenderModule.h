#pragma once

#include "Core/Modules/ModuleInterface.h"

constexpr size_t GRenderInfoCount = 3;

class IRenderModule : public IModuleInterface
{
public:
	virtual bool Init() noexcept = 0;
	virtual void Clear() noexcept = 0;

	virtual void BeginFrame_GameThread() noexcept = 0;
	virtual void EndFrame_GameThread() noexcept = 0;

	virtual size_t GetFrameInfoIndex_GameThread() noexcept = 0;
	virtual size_t GetFrameInfoIndex_RenderThread() noexcept = 0;
};

IRenderModule* GetRenderModule() noexcept;
