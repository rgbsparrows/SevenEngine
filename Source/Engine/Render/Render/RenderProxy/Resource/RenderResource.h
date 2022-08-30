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
	EVertexSemanticFlag mVertexSemantic;

	IRDIBuffer* mIndexBuffer;
	IRDIBuffer* mVertexBuffer[EnumToInt(EVertexSemantic::Num)] = {};

	std::vector<SRange> mSubMeshRange;
};
template<> struct RRenderInfoTraits<RMesh> : RShareMode {};

struct RMeshData
{
	EVertexSemanticFlag mVertexSemantic;

	const std::vector<uint32_t>* mIndexBuffer = nullptr;

	const std::vector<Math::SFloat3>* mPositionBuffer = nullptr;
	const std::vector<Math::SFloat4>* mColorBuffer = nullptr;
	const std::vector<Math::SFloat3>* mNormalBuffer = nullptr;
	const std::vector<Math::SFloat4>* mTangentBuffer = nullptr;
	const std::vector<Math::SUShort4>* mBlendIndicesBuffer = nullptr;
	const std::vector<Math::SFloat4>* mBlendWeightBuffer = nullptr;
	const std::vector<Math::SFloat2>* mUvBuffer[8] = {};

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
