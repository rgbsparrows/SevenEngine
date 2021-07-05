#pragma once
#include "Render/RenderModule.h"

template<typename _renderInfoType, bool _isFrameExclusive = true>
struct SRenderProxy
{
public:
	using RenderInfoType = _renderInfoType;
	static constexpr bool IsFrameExclusive = _isFrameExclusive;
	static constexpr size_t RenderInfoCount = IsFrameExclusive ? GRenderInfoCount : 1;

	RenderInfoType& Get_GameThread() noexcept
	{
		return mRenderInfo[GetRenderModule()->GetFrameInfoIndex_GameThread()];
	}

	RenderInfoType& Get_RenderThread() noexcept
	{
		return mRenderInfo[GetRenderModule()->GetFrameInfoIndex_RenderThread()];
	}

private:
	RenderInfoType mRenderInfo[RenderInfoCount];
};
