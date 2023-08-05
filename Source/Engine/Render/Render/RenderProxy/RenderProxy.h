#pragma once
#include "Render/RenderModule.h"

#include <concepts>

class SRenderContext;

enum class EExclusiveMode
{
	Exclusive,
	Share,
};

struct RExclusiveMode {};

struct RShareMode {};

template<typename _renderInfoType>
struct RRenderInfoTraits {};

template<typename _renderInfoType>
concept CExclusiveRenderInfo = std::default_initializable<_renderInfoType> && std::derived_from<RRenderInfoTraits<_renderInfoType>, RExclusiveMode>;

template<typename _renderInfoType>
concept CShareRenderInfo = std::default_initializable<_renderInfoType> && std::derived_from<RRenderInfoTraits<_renderInfoType>, RShareMode>;

template<typename _renderInfoType>
concept CRenderInfo = CExclusiveRenderInfo<_renderInfoType> || CShareRenderInfo<_renderInfoType>;

template<typename _renderInfoType>
concept CInitableRenderInfo = CRenderInfo<_renderInfoType> && requires(_renderInfoType & _renderInfo, SRenderContext& _renderContext) { _renderInfo.Init(_renderContext); };

template<typename _renderInfoType>
concept CClearableRenderInfo = CRenderInfo<_renderInfoType> && requires(_renderInfoType & _renderInfo, SRenderContext& _renderContext) { _renderInfo.Clear(_renderContext); };

__interface RRenderProxyBase
{
	void Init(SRenderContext& _renderContext) noexcept;
	void Release(SRenderContext& _renderContext) noexcept;
};

template<CRenderInfo _renderInfoType>
class RRenderProxy : public RRenderProxyBase
{
public:
	using RenderInfoType = _renderInfoType;
	static constexpr EExclusiveMode ExclusiveMode = CExclusiveRenderInfo<RenderInfoType> ? EExclusiveMode::Exclusive : EExclusiveMode::Share;
	static constexpr size_t RenderInfoCount = (ExclusiveMode == EExclusiveMode::Exclusive) ? GRenderInfoCount : 1;
	static constexpr bool HasClearMethod = CClearableRenderInfo<RenderInfoType>;
	static constexpr bool HasInitMethod = CInitableRenderInfo<RenderInfoType>;

	RenderInfoType& Get_GameThread() noexcept
	{
		if constexpr (ExclusiveMode == EExclusiveMode::Share)
			return mRenderInfo[0];
		else
			return mRenderInfo[GetRenderModule()->GetFrameInfoIndex_GameThread()];
	}

	RenderInfoType& Get_RenderThread() noexcept
	{
		if constexpr (ExclusiveMode == EExclusiveMode::Share)
			return mRenderInfo[0];
		else
			return mRenderInfo[GetRenderModule()->GetFrameInfoIndex_RenderThread()];
	}

	RenderInfoType& Get() noexcept
	{
		if constexpr (ExclusiveMode == EExclusiveMode::Share)
			return mRenderInfo[0];
		else
			return *static_cast<RenderInfoType*>(nullptr);
	}

	void Init(SRenderContext& _renderContext) noexcept override
	{
		if constexpr (HasInitMethod)
		{
			for (size_t i = 0; i != RenderInfoCount; ++i)
				mRenderInfo[i].Init(_renderContext);
		}
	}

	void Release(SRenderContext& _renderContext) noexcept override
	{
		if constexpr (HasClearMethod)
		{
			for (size_t i = 0; i != RenderInfoCount; ++i)
				mRenderInfo[i].Clear(_renderContext);
		}

		delete this;
	}

private:
	RenderInfoType mRenderInfo[RenderInfoCount];
};

class RDirtyFlag
{
public:
	RDirtyFlag(bool _isDirty = true) noexcept
		: mDirtyFrameCount(_isDirty ? GRenderInfoCount : 0)
	{
	}

	bool IsDirty() const noexcept { return mDirtyFrameCount != 0; }

	void MarkDirty() noexcept { mDirtyFrameCount = GRenderInfoCount; }

	void Update() noexcept
	{
		if (mDirtyFrameCount != 0)
			--mDirtyFrameCount;
	}

private:
	size_t mDirtyFrameCount = 0;
};

