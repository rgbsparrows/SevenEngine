#pragma once
#include "Core/Math/Type.h"
#include "RDI/RDIFunctionHelper.h"
#include "Render/RenderProxy/RenderProxy.h"
#include "Render/RenderProxy/Resource/RenderResource.h"
#include "RDI/Interface/RDIDescriptorHeapRange.h"

#include <vector>

__interface IRDISwapChain;
__interface IRDIDescriptorHeapRange;

struct RSwapChainData
{
	uint32_t mWidth = 0;
	uint32_t mHeight = 0;
	Math::SUIntFraction mRefreshRate;
	ERDIPixelFormat mPixelFormat = ERDIPixelFormat::UNKNOWN;
	ERDIScalingMode mScalingMode = ERDIScalingMode::Stretched;
	HWND mOutputWindow = nullptr;
	bool mIsWindowed = true;

	bool mNeedResize = false;
};

struct RSwapChain
{
	IRDISwapChain* mSwapChain = nullptr;
};
template<> struct RRenderInfoTraits<RSwapChain> : RShareMode {};

struct RImguiTexture2D
{
	IRDITexture2D* mTexture2D = nullptr;
	IRDIDescriptorHeapRange* mDescriptorHeapRange = nullptr;

	void Clear()
	{
		if(mDescriptorHeapRange)
			mDescriptorHeapRange->Release();
	}
};
template<> struct RRenderInfoTraits<RImguiTexture2D> : RShareMode {};

struct RImguiVertex
{
	Math::SFloat2 mPos;
	Math::SFloat2 mUv;
	Math::SUColor32 mColor = 0;
};

struct RImguiDrawCmd
{
	Math::SIntRect mClipRect;
	RRenderProxy<RImguiTexture2D>* mTextureId = nullptr;
	uint32_t mVertexOffset = 0;
	uint32_t mIndexOffset = 0;
	uint32_t mVertexCount = 0;
};

struct RImguiDrawData
{
	std::vector<RImguiDrawCmd> mCmdBuffer;
	std::vector<RImguiVertex> mVertexBuffer;
	std::vector<uint16_t> mIndexBuffer;
	Math::SFloat2 mDisplayPos;
	Math::SFloat2 mDisplaySize;

	IRDIBuffer* mRDIIndexBuffer = nullptr;
	IRDIBuffer* mRDIVertexBuffer = nullptr;

	void NewFrame() noexcept
	{
		mCmdBuffer.resize(0);
		mVertexBuffer.resize(0);
		mIndexBuffer.resize(0);
		mDisplayPos = Math::SFloat2();
		mDisplaySize = Math::SFloat2();
	}
};
template<> struct RRenderInfoTraits<RImguiDrawData> : RExclusiveMode {};
