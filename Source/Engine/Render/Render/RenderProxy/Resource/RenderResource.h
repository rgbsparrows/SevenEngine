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
	Color,
	Normal,
	Tangent,
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
	EVertexSemanticFlag mVertexSemantic = EVertexSemanticFlag::None;

	IRDIBuffer* mIndexBuffer = nullptr;
	IRDIBuffer* mVertexBuffer[EnumToInt(EVertexSemantic::Num)] = {};

	std::vector<SRange> mSubMeshRange;
};
template<> struct RRenderInfoTraits<RMesh> : RShareMode {};

struct RMeshData
{
	EVertexSemanticFlag mVertexSemantic;

	std::vector<uint32_t> mIndexBuffer;

	std::vector<Math::SFloat3> mPositionBuffer;
	std::vector<Math::SFColor> mColorBuffer;
	std::vector<Math::SFloat3> mNormalBuffer;
	std::vector<Math::SFloat4> mTangentBuffer;
	std::vector<Math::SUShort4> mBlendIndicesBuffer;
	std::vector<Math::SFloat4> mBlendWeightBuffer;
	std::vector<Math::SFloat2> mUvBuffer[8];

	std::vector<SRange> mSubMeshRange;
};

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

	std::vector<SRange> mSubresourceData;
	SBlob mResourceData;
};

#pragma endregion
