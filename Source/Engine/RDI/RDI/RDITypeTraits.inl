#pragma once

#include "RDI/Interface/RDIResource.h"
#include "RDIFunctionHelper.h"

#include <concepts>

namespace RDITypeTraitsDelta
{
	template<typename _rdiResourceType>
	concept CRDIResource = std::derived_from<_rdiResourceType, IRDIResource>;

	template<CRDIResource _rdiResourceType>
	struct TRDIResourceDescType {};

	template<>
	struct TRDIResourceDescType<IRDIBuffer>
	{
		using Type = SRDIBufferResourceDesc;
	};

	template<>
	struct TRDIResourceDescType<IRDITexture1D>
	{
		using Type = SRDITexture1DResourceDesc;
	};

	template<>
	struct TRDIResourceDescType<IRDITexture1DArray>
	{
		using Type = SRDITexture1DArrayResourceDesc;
	};

	template<>
	struct TRDIResourceDescType<IRDITexture2D>
	{
		using Type = SRDITexture2DResourceDesc;
	};

	template<>
	struct TRDIResourceDescType<IRDITexture2DArray>
	{
		using Type = SRDITexture2DArrayResourceDesc;
	};

	template<>
	struct TRDIResourceDescType<IRDITexture3D>
	{
		using Type = SRDITexture3DResourceDesc;
	};

	template<>
	struct TRDIResourceDescType<IRDITextureCube>
	{
		using Type = SRDITextureCubeResourceDesc;
	};

	template<>
	struct TRDIResourceDescType<IRDITextureCubeArray>
	{
		using Type = SRDITextureCubeArrayResourceDesc;
	};

	template<>
	struct TRDIResourceDescType<IRDISampler>
	{
		using Type = SRDISamplerResourceDesc;
	};

	template<typename _rdiResourceDescType>
	concept CRDIResourceDescType =
		std::is_same_v<_rdiResourceDescType, SRDIBufferResourceDesc> ||
		std::is_same_v<_rdiResourceDescType, SRDITexture1DResourceDesc> ||
		std::is_same_v<_rdiResourceDescType, SRDITexture1DArrayResourceDesc> ||
		std::is_same_v<_rdiResourceDescType, SRDITexture2DResourceDesc> ||
		std::is_same_v<_rdiResourceDescType, SRDITexture2DArrayResourceDesc> ||
		std::is_same_v<_rdiResourceDescType, SRDITexture3DResourceDesc> ||
		std::is_same_v<_rdiResourceDescType, SRDITextureCubeResourceDesc> ||
		std::is_same_v<_rdiResourceDescType, SRDITextureCubeArrayResourceDesc> ||
		std::is_same_v<_rdiResourceDescType, SRDISamplerResourceDesc>;
}
