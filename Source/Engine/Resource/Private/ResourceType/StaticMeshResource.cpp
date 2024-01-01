#include "Render/RenderCommandList.h"
#include "Resource/ResourceType/StaticMeshResource.h"

void SStaticMeshResource::Serialize(SWriteStream& _writeStream) const noexcept
{
	_writeStream.Write(mVertexSemantic);
	_writeStream.Write(mIndexBuffer);
	_writeStream.Write(mPositionBuffer);
	_writeStream.Write(mColorBuffer);
	_writeStream.Write(mNormalBuffer);
	_writeStream.Write(mTangentBuffer);
	_writeStream.Write(mBlendIndicesBuffer);
	_writeStream.Write(mBlendWeightBuffer);
	for (size_t i = 0; i != 8; ++i)
		_writeStream.Write(mUvBuffer[i]);

	_writeStream.Write(mSubMeshList);
}

bool SStaticMeshResource::Deserialize(SReadStream& _readStream)
{
	_readStream.Read(mVertexSemantic);
	_readStream.Read(mIndexBuffer);
	_readStream.Read(mPositionBuffer);
	_readStream.Read(mColorBuffer);
	_readStream.Read(mNormalBuffer);
	_readStream.Read(mTangentBuffer);
	_readStream.Read(mBlendIndicesBuffer);
	_readStream.Read(mBlendWeightBuffer);
	for (size_t i = 0; i != 8; ++i)
		_readStream.Read(mUvBuffer[i]);

	_readStream.Read(mSubMeshList);

	return true;
}

void SStaticMeshResource::RefrashContent() noexcept
{
	if (mMeshProxy == nullptr)
		mMeshProxy = new RRenderProxy<RMesh>;

	RMeshData meshData;
	meshData.mVertexSemantic = mVertexSemantic;

	meshData.mIndexBuffer = mIndexBuffer;

	for (size_t i = 0; i != mSubMeshList.size(); ++i)
		meshData.mSubMeshRange.push_back(mSubMeshList[i].mSubMeshRange);

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Position)) != EVertexSemanticFlag::None)
		meshData.mPositionBuffer = mPositionBuffer;

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Color)) != EVertexSemanticFlag::None)
		meshData.mColorBuffer = mColorBuffer;

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Normal)) != EVertexSemanticFlag::None)
		meshData.mNormalBuffer = mNormalBuffer;

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Tangent)) != EVertexSemanticFlag::None)
		meshData.mTangentBuffer = mTangentBuffer;

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::BlendIndices)) != EVertexSemanticFlag::None)
		meshData.mBlendIndicesBuffer = mBlendIndicesBuffer;

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::BlendWeight)) != EVertexSemanticFlag::None)
		meshData.mBlendWeightBuffer = mBlendWeightBuffer;

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Uv0)) != EVertexSemanticFlag::None)
		meshData.mUvBuffer[0] = mUvBuffer[0];

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Uv1)) != EVertexSemanticFlag::None)
		meshData.mUvBuffer[1] = mUvBuffer[1];

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Uv2)) != EVertexSemanticFlag::None)
		meshData.mUvBuffer[2] = mUvBuffer[2];

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Uv3)) != EVertexSemanticFlag::None)
		meshData.mUvBuffer[3] = mUvBuffer[3];

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Uv4)) != EVertexSemanticFlag::None)
		meshData.mUvBuffer[4] = mUvBuffer[4];

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Uv5)) != EVertexSemanticFlag::None)
		meshData.mUvBuffer[5] = mUvBuffer[5];

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Uv6)) != EVertexSemanticFlag::None)
		meshData.mUvBuffer[6] = mUvBuffer[6];

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Uv7)) != EVertexSemanticFlag::None)
		meshData.mUvBuffer[7] = mUvBuffer[7];

	meshData.mSubMeshRange.resize(mSubMeshList.size());

	for (size_t i = 0; i != mSubMeshList.size(); ++i)
		meshData.mSubMeshRange[i] = mSubMeshList[i].mSubMeshRange;

	GetRenderCommandList()->RefrashMesh_I(mMeshProxy, meshData);
}

void SStaticMeshResource::SetVertexSemantic(EVertexSemanticFlag _vertexSemanticFlag) noexcept
{
	mVertexSemantic = _vertexSemanticFlag;

	size_t vertexCount = mPositionBuffer.size();

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Position)) != EVertexSemanticFlag::None)
		mPositionBuffer.resize(vertexCount);
	else mPositionBuffer.clear();

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Color)) != EVertexSemanticFlag::None)
		mColorBuffer.resize(vertexCount);
	else mColorBuffer.clear();

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Normal)) != EVertexSemanticFlag::None)
		mNormalBuffer.resize(vertexCount);
	else mNormalBuffer.clear();

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Tangent)) != EVertexSemanticFlag::None)
		mTangentBuffer.resize(vertexCount);
	else mTangentBuffer.clear();

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::BlendIndices)) != EVertexSemanticFlag::None)
		mBlendIndicesBuffer.resize(vertexCount);
	else mBlendIndicesBuffer.clear();

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::BlendWeight)) != EVertexSemanticFlag::None)
		mBlendWeightBuffer.resize(vertexCount);
	else mBlendWeightBuffer.clear();

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Uv0)) != EVertexSemanticFlag::None)
		mUvBuffer[0].resize(vertexCount);
	else mUvBuffer[0].clear();

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Uv1)) != EVertexSemanticFlag::None)
		mUvBuffer[1].resize(vertexCount);
	else mUvBuffer[1].clear();

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Uv2)) != EVertexSemanticFlag::None)
		mUvBuffer[2].resize(vertexCount);
	else mUvBuffer[2].clear();

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Uv3)) != EVertexSemanticFlag::None)
		mUvBuffer[3].resize(vertexCount);
	else mUvBuffer[3].clear();

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Uv4)) != EVertexSemanticFlag::None)
		mUvBuffer[4].resize(vertexCount);
	else mUvBuffer[4].clear();

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Uv5)) != EVertexSemanticFlag::None)
		mUvBuffer[5].resize(vertexCount);
	else mUvBuffer[5].clear();

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Uv6)) != EVertexSemanticFlag::None)
		mUvBuffer[6].resize(vertexCount);
	else mUvBuffer[6].clear();

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Uv7)) != EVertexSemanticFlag::None)
		mUvBuffer[7].resize(vertexCount);
	else mUvBuffer[7].clear();
}

void SStaticMeshResource::ResizeVertexCount(size_t _vertexCount) noexcept
{
	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Position)) != EVertexSemanticFlag::None)
		mPositionBuffer.resize(_vertexCount);

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Color)) != EVertexSemanticFlag::None)
		mColorBuffer.resize(_vertexCount);

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Normal)) != EVertexSemanticFlag::None)
		mNormalBuffer.resize(_vertexCount);

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Tangent)) != EVertexSemanticFlag::None)
		mTangentBuffer.resize(_vertexCount);

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::BlendIndices)) != EVertexSemanticFlag::None)
		mBlendIndicesBuffer.resize(_vertexCount);

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::BlendWeight)) != EVertexSemanticFlag::None)
		mBlendWeightBuffer.resize(_vertexCount);

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Uv0)) != EVertexSemanticFlag::None)
		mUvBuffer[0].resize(_vertexCount);

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Uv1)) != EVertexSemanticFlag::None)
		mUvBuffer[1].resize(_vertexCount);

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Uv2)) != EVertexSemanticFlag::None)
		mUvBuffer[2].resize(_vertexCount);

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Uv3)) != EVertexSemanticFlag::None)
		mUvBuffer[3].resize(_vertexCount);

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Uv4)) != EVertexSemanticFlag::None)
		mUvBuffer[4].resize(_vertexCount);

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Uv5)) != EVertexSemanticFlag::None)
		mUvBuffer[5].resize(_vertexCount);

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Uv6)) != EVertexSemanticFlag::None)
		mUvBuffer[6].resize(_vertexCount);

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Uv7)) != EVertexSemanticFlag::None)
		mUvBuffer[7].resize(_vertexCount);
}

void SStaticMeshResource::ResizeTriangleCount(size_t _triangleCount) noexcept
{
	mIndexBuffer.resize(_triangleCount * 3);
}

void SStaticMeshResource::ResizeSubMeshCount(size_t _subMeshCount) noexcept
{
	mSubMeshList.resize(_subMeshCount);
}

void SStaticMeshResource::SetVertex(const SFullVertex& _vertex, size_t _vertexIndex) noexcept
{
	CHECK(_vertexIndex < mPositionBuffer.size());

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Position)) != EVertexSemanticFlag::None)
		mPositionBuffer[_vertexIndex] = _vertex.mPosition;

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Color)) != EVertexSemanticFlag::None)
		mColorBuffer[_vertexIndex] = _vertex.mColor;

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Normal)) != EVertexSemanticFlag::None)
		mNormalBuffer[_vertexIndex] = _vertex.mNormal;

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Tangent)) != EVertexSemanticFlag::None)
		mTangentBuffer[_vertexIndex] = _vertex.mTangent;

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::BlendIndices)) != EVertexSemanticFlag::None)
		mBlendIndicesBuffer[_vertexIndex] = _vertex.mBlendIndices;

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::BlendWeight)) != EVertexSemanticFlag::None)
		mBlendWeightBuffer[_vertexIndex] = _vertex.mBlendWeight;

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Uv0)) != EVertexSemanticFlag::None)
		mUvBuffer[0][_vertexIndex] = _vertex.mUv[0];

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Uv1)) != EVertexSemanticFlag::None)
		mUvBuffer[1][_vertexIndex] = _vertex.mUv[1];

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Uv2)) != EVertexSemanticFlag::None)
		mUvBuffer[2][_vertexIndex] = _vertex.mUv[2];

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Uv3)) != EVertexSemanticFlag::None)
		mUvBuffer[3][_vertexIndex] = _vertex.mUv[3];

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Uv4)) != EVertexSemanticFlag::None)
		mUvBuffer[4][_vertexIndex] = _vertex.mUv[4];

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Uv5)) != EVertexSemanticFlag::None)
		mUvBuffer[5][_vertexIndex] = _vertex.mUv[5];

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Uv6)) != EVertexSemanticFlag::None)
		mUvBuffer[6][_vertexIndex] = _vertex.mUv[6];

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Uv7)) != EVertexSemanticFlag::None)
		mUvBuffer[7][_vertexIndex] = _vertex.mUv[7];
}

SFullVertex SStaticMeshResource::GetVertex(size_t _vertexIndex) const noexcept
{
	CHECK(_vertexIndex < mPositionBuffer.size());

	SFullVertex vertex;

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Position)) != EVertexSemanticFlag::None)
		vertex.mPosition = mPositionBuffer[_vertexIndex];

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Color)) != EVertexSemanticFlag::None)
		vertex.mColor = mColorBuffer[_vertexIndex];

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Normal)) != EVertexSemanticFlag::None)
		vertex.mNormal = mNormalBuffer[_vertexIndex];

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Tangent)) != EVertexSemanticFlag::None)
		vertex.mTangent = mTangentBuffer[_vertexIndex];

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::BlendIndices)) != EVertexSemanticFlag::None)
		vertex.mBlendIndices = mBlendIndicesBuffer[_vertexIndex];

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::BlendWeight)) != EVertexSemanticFlag::None)
		vertex.mBlendWeight = mBlendWeightBuffer[_vertexIndex];

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Uv0)) != EVertexSemanticFlag::None)
		vertex.mUv[0] = mUvBuffer[0][_vertexIndex];

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Uv1)) != EVertexSemanticFlag::None)
		vertex.mUv[1] = mUvBuffer[1][_vertexIndex];

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Uv2)) != EVertexSemanticFlag::None)
		vertex.mUv[2] = mUvBuffer[2][_vertexIndex];

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Uv3)) != EVertexSemanticFlag::None)
		vertex.mUv[3] = mUvBuffer[3][_vertexIndex];

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Uv4)) != EVertexSemanticFlag::None)
		vertex.mUv[4] = mUvBuffer[4][_vertexIndex];

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Uv5)) != EVertexSemanticFlag::None)
		vertex.mUv[5] = mUvBuffer[5][_vertexIndex];

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Uv6)) != EVertexSemanticFlag::None)
		vertex.mUv[6] = mUvBuffer[6][_vertexIndex];

	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Uv7)) != EVertexSemanticFlag::None)
		vertex.mUv[7] = mUvBuffer[7][_vertexIndex];

	return vertex;
}

void SStaticMeshResource::SetTriangle(const SMeshTriangle& _triangle, size_t _triangleIndex) noexcept
{
	CHECK(_triangleIndex * 3 + 2 < mIndexBuffer.size());

	mIndexBuffer[_triangleIndex * 3 + 0] = _triangle[0];
	mIndexBuffer[_triangleIndex * 3 + 1] = _triangle[1];
	mIndexBuffer[_triangleIndex * 3 + 2] = _triangle[2];
}

SMeshTriangle SStaticMeshResource::GetTriangle(size_t _triangleIndex) const noexcept
{
	CHECK(_triangleIndex * 3 + 2 < mIndexBuffer.size());

	SMeshTriangle triangle;
	triangle[0] = mIndexBuffer[0];
	triangle[1] = mIndexBuffer[1];
	triangle[2] = mIndexBuffer[2];

	return triangle;
}

void SStaticMeshResource::SetSubMesh(const SSubMesh& _subMesh, size_t _subMeshIndex) noexcept
{
	CHECK(_subMeshIndex < mSubMeshList.size());

	mSubMeshList[_subMeshIndex] = _subMesh;
}

SSubMesh SStaticMeshResource::GetSubMesh(size_t _subMeshIndex) const noexcept
{
	CHECK(_subMeshIndex < mSubMeshList.size());

	return mSubMeshList[_subMeshIndex];
}

Math::SFloat3* SStaticMeshResource::GetPositionBuffer() noexcept
{
	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Position)) != EVertexSemanticFlag::None)
		return &mPositionBuffer[0];

	return nullptr;
}

Math::SFColor* SStaticMeshResource::GetColorBuffer() noexcept
{
	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Color)) != EVertexSemanticFlag::None)
		return &mColorBuffer[0];

	return nullptr;
}

Math::SFloat3* SStaticMeshResource::GetNormalBuffer() noexcept
{
	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Normal)) != EVertexSemanticFlag::None)
		return &mNormalBuffer[0];

	return nullptr;
}

Math::SFloat4* SStaticMeshResource::GetTangentBuffer() noexcept
{
	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::Tangent)) != EVertexSemanticFlag::None)
		return &mTangentBuffer[0];

	return nullptr;
}

Math::SUShort4* SStaticMeshResource::GetBlendIndicesBuffer() noexcept
{
	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::BlendIndices)) != EVertexSemanticFlag::None)
		return &mBlendIndicesBuffer[0];

	return nullptr;
}

Math::SFloat4* SStaticMeshResource::GetBlendWeightBuffer() noexcept
{
	if ((mVertexSemantic & ConvertToEnumFlag(EVertexSemantic::BlendIndices)) != EVertexSemanticFlag::None)
		return &mBlendWeightBuffer[0];

	return nullptr;
}

Math::SFloat2* SStaticMeshResource::GetUvBuffer(size_t _uvChannel) noexcept
{
	CHECK(_uvChannel < 8);

	EVertexSemantic vertexSemantic = static_cast<EVertexSemantic>(EnumToInt(EVertexSemantic::Uv0) + _uvChannel);

	if ((mVertexSemantic & ConvertToEnumFlag(vertexSemantic)) != EVertexSemanticFlag::None)
		return &mUvBuffer[_uvChannel][0];

	return nullptr;
}

const std::vector<Math::SFloat2>& SStaticMeshResource::GetUvBuffer(size_t _uvChannel) const noexcept
{
	CHECK(_uvChannel < 8);

	EVertexSemantic vertexSemantic = static_cast<EVertexSemantic>(EnumToInt(EVertexSemantic::Uv0) + _uvChannel);

	return mUvBuffer[_uvChannel];
}

void SStaticMeshResource::OnRelease() noexcept
{
	if (mMeshProxy)
	{
		GetRenderCommandList()->AddExpiringRenderProxy(mMeshProxy);
		mMeshProxy = nullptr;
	}

	BaseClass::OnRelease();
}
