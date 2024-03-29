#pragma once

#include "Core/Math/Type.h"
#include "Core/Container/Blob.h"
#include "Resource/ResourceBase.h"
#include "Resource/ResourceProxy.h"
#include "Core/Util/TemplateUtil.h"
#include "Render/RenderProxy/Resource/RenderResource.h"

#include <vector>

struct SFullVertex 
{
	Math::SFloat3 mPosition;
	Math::SFColor mColor;
	Math::SFloat3 mNormal;
	Math::SFloat4 mTangent;
	Math::SUShort4 mBlendIndices;
	Math::SFloat4 mBlendWeight;
	Math::SFloat2 mUv[8];

	bool operator== (const SFullVertex&)const = default;
};

struct SMeshTriangle
{
	SMeshTriangle() noexcept = default;
	SMeshTriangle(uint32_t _i0, uint32_t _i1, uint32_t _i2) noexcept
	{
		mVertexIndex[0] = _i0;
		mVertexIndex[1] = _i1;
		mVertexIndex[2] = _i2;
	}

	uint32_t mVertexIndex[3] = {};

	uint32_t operator[] (size_t _index) const noexcept { return mVertexIndex[_index]; }
	uint32_t& operator[] (size_t _index) noexcept { return mVertexIndex[_index]; }
};

struct SSubMesh
{
	SSubMesh() noexcept = default;
	SSubMesh(SRange _subMeshRange, const std::string& _subMeshName)
		: mSubMeshRange(_subMeshRange), mSubMeshName(_subMeshName)
	{
	}

	SRange mSubMeshRange;
	std::string mSubMeshName;
};

template<>
struct TSerialize<SSubMesh>
{
	using ValueType = SSubMesh;

	void Serialize(SWriteStream& _writeStream, const ValueType& _value) noexcept
	{
		_writeStream.Write(_value.mSubMeshRange);
		_writeStream.Write(_value.mSubMeshName);
	}

	void Deserialize(SReadStream& _readStream, ValueType& _value) noexcept
	{
		_readStream.Read(_value.mSubMeshRange);
		_readStream.Read(_value.mSubMeshName);
	}
};

class SStaticMeshResource : public SResourceBase
{
	DECLARE_CLASSOBJECT_BODY(SStaticMeshResource, SResourceBase)

public:
	void Serialize(SWriteStream& _writeStream)const noexcept override;
	bool Deserialize(SReadStream& _readStream) override;

	void RefrashContent() noexcept override;

	RRenderProxy<RMesh>* GetRenderProxy() noexcept { return mMeshProxy; }

	void SetVertexSemantic(EVertexSemanticFlag _vertexSemanticFlag) noexcept;

	void ResizeVertexCount(size_t _vertexCount) noexcept;
	void ResizeTriangleCount(size_t _triangleCount) noexcept;
	void ResizeSubMeshCount(size_t _subMeshCount) noexcept;

	void SetVertex(const SFullVertex& _vertex, size_t _vertexIndex) noexcept;
	SFullVertex GetVertex(size_t _vertexIndex)const noexcept;
	void SetTriangle(const SMeshTriangle& _triangle, size_t _triangleIndex) noexcept;
	SMeshTriangle GetTriangle(size_t _triangleIndex) const noexcept;

	void SetSubMesh(const SSubMesh& _subMesh, size_t _subMeshIndex) noexcept;
	SSubMesh GetSubMesh(size_t _subMeshIndex) const noexcept;

	uint32_t* GetIndexBuffer() noexcept { return &mIndexBuffer[0]; }
	Math::SFloat3* GetPositionBuffer() noexcept;
	Math::SFColor* GetColorBuffer() noexcept;
	Math::SFloat3* GetNormalBuffer() noexcept;
	Math::SFloat4* GetTangentBuffer() noexcept;
	Math::SUShort4* GetBlendIndicesBuffer() noexcept;
	Math::SFloat4* GetBlendWeightBuffer() noexcept;
	Math::SFloat2* GetUvBuffer(size_t _uvChannel) noexcept;

	const std::vector<uint32_t>& GetIndexBuffer()const noexcept { return mIndexBuffer; }
	const std::vector<Math::SFloat3>& GetPositionBuffer()const noexcept { return mPositionBuffer; }
	const std::vector<Math::SFColor>& GetColorBuffer()const noexcept { return mColorBuffer; }
	const std::vector<Math::SFloat3>& GetNormalBuffer()const noexcept { return mNormalBuffer; }
	const std::vector<Math::SFloat4>& GetTangentBuffer()const noexcept { return mTangentBuffer; }
	const std::vector<Math::SUShort4>& GetBlendIndicesBuffer()const noexcept { return mBlendIndicesBuffer; }
	const std::vector<Math::SFloat4>& GetBlendWeightBuffer()const noexcept { return mBlendWeightBuffer; }
	const std::vector<Math::SFloat2>& GetUvBuffer(size_t _uvChannel)const noexcept;

	const std::vector<SSubMesh>& GetSubMeshList() noexcept { return mSubMeshList; }

protected:
	void OnRelease() noexcept override;

public:
	EVertexSemanticFlag mVertexSemantic = ConvertToEnumFlag({
						EVertexSemantic::Position,
						EVertexSemantic::Color,
						EVertexSemantic::Normal,
						EVertexSemantic::Tangent,
						EVertexSemantic::Uv0 }
	);

	std::vector<uint32_t> mIndexBuffer;

	std::vector<Math::SFloat3> mPositionBuffer;
	std::vector<Math::SFColor> mColorBuffer;
	std::vector<Math::SFloat3> mNormalBuffer;
	std::vector<Math::SFloat4> mTangentBuffer;
	std::vector<Math::SUShort4> mBlendIndicesBuffer;
	std::vector<Math::SFloat4> mBlendWeightBuffer;
	std::vector<Math::SFloat2> mUvBuffer[8];

	std::vector<SSubMesh> mSubMeshList;

	RRenderProxy<RMesh>* mMeshProxy = nullptr;
};

using SStaticMeshRP = TResourceProxy<SStaticMeshResource>;
