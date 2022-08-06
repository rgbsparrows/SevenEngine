#pragma once
#include "Core/Container/Blob.h"
#include "RDI/RDIFunctionHelper.h"
#include "RDI/Interface/RDIResource.h"
#include "Render/RenderProxy/RenderProxy.h"

__interface IRDIGraphicsPipelineState;
__interface IRDIComputePipelineState;

#pragma region Mesh

enum class EVertexSemantic
{
	Position,
	Normal,
	Tangent,
	Color,
	BlendIndices,
	BlendWeight,
	Uv0,
	Uv1,
	Uv2,
	Uv3,
	Uv4,
	Uv5,
	Uv6,
	Uv7,

	Num
};

enum class EVertexSemanticFlag : uint32_t
{
	None = 0,
	Error = TErrorEnumValue<EVertexSemanticFlag>,
};

REGIST_ENUM_FLAG_FORM_ENUM(EVertexSemanticFlag, EVertexSemantic)

struct RMesh
{
	EVertexSemanticFlag mVertexSemantic;

	IRDIBuffer* mIndexBuffer;
	IRDIBuffer* mVertexBuffer[EnumToInt(EVertexSemantic::Num)] = {};
};
template<> struct RRenderInfoTraits<RMesh> : RShareMode {};

struct RMeshData
{
	bool mIsDynamicMesh;

	std::vector<Math::SFloat3> mPositionBuffer;
	std::vector<Math::SFloat3> mNormalBuffer;
	std::vector<Math::SFloat4> mTangentBuffer;
	std::vector<Math::SFloat4> mColorBuffer;
	std::vector<Math::SInt4> mBlendIndices;
	std::vector<Math::SFloat4> mBlendWeight;
	std::vector<Math::SFloat2> mUv[8];
};
template<> struct RRenderInfoTraits<RMeshData> : RExclusiveMode {};

#pragma endregion


#pragma region Texture

struct RTexture2D
{
	IRDITexture2D* mTexture = nullptr;
};
template<> struct RRenderInfoTraits<RTexture2D> : RShareMode {};

struct RTexture2DData
{
	SRDITexture2DResourceDesc mDesc;

	std::vector<SBufferView> mSubresourceData;
	SBlob mResourceData;
};
template<> struct RRenderInfoTraits<RTexture2DData> : RExclusiveMode {};

struct RRenderTarget2D
{
	IRDITexture2D* mRenderTarget;
};

#pragma endregion
