#include "RDI/RDIPixelFormat.h"
#include "Core/Math/Math.h"

SPixelFormatMeta SPixelFormatMeta::GetPixelFormatMeta(ERDIPixelFormat _pixelFormat)
{
	TODO("ÍêÉÆPixelFormatMetaÓ³Éä")
	static constexpr SPixelFormatMeta pixelFormatMetaMap[EnumToInt(ERDIPixelFormat::Num)] = {
		//Name,							BlockSizeX, BlockSizeY, BlockSizeZ, BlockBytes, NumComponents,	Flag
		{L"UNKNOW",						0,			0,			0,			0,			0,				EPixelFormatFlag::Error								},
		{L"R32G32B32A32_TYPELESS",		1,			1,			1,			16,			4,				EPixelFormatFlag::RGBA | EPixelFormatFlag::TYPELESS	},
		{L"R32G32B32A32_FLOAT",			1,			1,			1,			16,			4,				EPixelFormatFlag::RGBA | EPixelFormatFlag::FLOAT	},
		{L"R32G32B32A32_UINT",			1,			1,			1,			16,			4,				EPixelFormatFlag::RGBA | EPixelFormatFlag::UINT		},
		{L"R32G32B32A32_SINT",			1,			1,			1,			16,			4,				EPixelFormatFlag::RGBA | EPixelFormatFlag::SINT		},
		{L"R32G32B32_TYPELESS",			1,			1,			1,			12,			3,				EPixelFormatFlag::RGB | EPixelFormatFlag::TYPELESS	},
		{L"R32G32B32_FLOAT",			1,			1,			1,			12,			3,				EPixelFormatFlag::RGB | EPixelFormatFlag::FLOAT		},
		{L"R32G32B32_UINT",				1,			1,			1,			12,			3,				EPixelFormatFlag::RGB | EPixelFormatFlag::UINT		},
		{L"R32G32B32_SINT",				1,			1,			1,			12,			3,				EPixelFormatFlag::RGB | EPixelFormatFlag::SINT		},
		{L"R16G16B16A16_TYPELESS",		1,			1,			1,			8,			4,				EPixelFormatFlag::RGBA | EPixelFormatFlag::TYPELESS	},
		{L"R16G16B16A16_FLOAT",			1,			1,			1,			8,			4,				EPixelFormatFlag::RGBA | EPixelFormatFlag::FLOAT	},
		{L"R16G16B16A16_UNORM",			1,			1,			1,			8,			4,				EPixelFormatFlag::RGBA | EPixelFormatFlag::UNORM	},
		{L"R16G16B16A16_UINT",			1,			1,			1,			8,			4,				EPixelFormatFlag::RGBA | EPixelFormatFlag::UINT		},
		{L"R16G16B16A16_SNORM",			1,			1,			1,			8,			4,				EPixelFormatFlag::RGBA | EPixelFormatFlag::SNORM	},
		{L"R16G16B16A16_SINT",			1,			1,			1,			8,			4,				EPixelFormatFlag::RGBA | EPixelFormatFlag::SINT		},
		{L"R32G32_TYPELESS",			1,			1,			1,			8,			2,				EPixelFormatFlag::RG | EPixelFormatFlag::TYPELESS	},
		{L"R32G32_FLOAT",				1,			1,			1,			8,			2,				EPixelFormatFlag::RG | EPixelFormatFlag::FLOAT		},
		{L"R32G32_UINT",				1,			1,			1,			8,			2,				EPixelFormatFlag::RG | EPixelFormatFlag::UINT		},
		{L"R32G32_SINT",				1,			1,			1,			8,			2,				EPixelFormatFlag::RG | EPixelFormatFlag::SINT		},

		{L"R32G8X24_TYPELESS",			1,			1,			1,			8,			3,				EPixelFormatFlag::RG | EPixelFormatFlag::TYPELESS	},
		{L"D32_FLOAT_S8X24_UINT",		1,			1,			1,			8,			3,				EPixelFormatFlag::DS | EPixelFormatFlag::SPECIAL	},
		{L"R32_FLOAT_X8X24_TYPELESS",	1,			1,			1,			8,			3,				EPixelFormatFlag::R | EPixelFormatFlag::FLOAT		},
		{L"X32_TYPELESS_G8X24_UINT",	1,			1,			1,			8,			1,				EPixelFormatFlag::G | EPixelFormatFlag::UINT		},
		{L"R10G10B10A2_TYPELESS",		1,			1,			1,			4,			4,				EPixelFormatFlag::RGBA | EPixelFormatFlag::TYPELESS	},
		{L"R10G10B10A2_UNORM",			1,			1,			1,			4,			4,				EPixelFormatFlag::RGBA | EPixelFormatFlag::UNORM	},
		{L"R10G10B10A2_UINT",			1,			1,			1,			4,			4,				EPixelFormatFlag::RGBA | EPixelFormatFlag::UINT		},
		{L"R11G11B10_FLOAT",			1,			1,			1,			4,			3,				EPixelFormatFlag::RGB | EPixelFormatFlag::FLOAT		},
		{L"R8G8B8A8_TYPELESS",			1,			1,			1,			4,			4,				EPixelFormatFlag::RGBA | EPixelFormatFlag::TYPELESS	},
		{L"R8G8B8A8_UNORM",				1,			1,			1,			4,			4,				EPixelFormatFlag::RGBA | EPixelFormatFlag::UNORM	},
		{L"R8G8B8A8_UNORM_SRGB",		1,			1,			1,			4,			4,				EPixelFormatFlag::RGBA | EPixelFormatFlag::UNORM | EPixelFormatFlag::SRGB	},
		{L"R8G8B8A8_UINT",				1,			1,			1,			4,			4,				EPixelFormatFlag::RGBA | EPixelFormatFlag::UINT		},
		{L"R8G8B8A8_SNORM",				1,			1,			1,			4,			4,				EPixelFormatFlag::RGBA | EPixelFormatFlag::SNORM	},
		{L"R8G8B8A8_SINT",				1,			1,			1,			4,			4,				EPixelFormatFlag::RGBA | EPixelFormatFlag::SINT		},
		{L"R16G16_TYPELESS",			1,			1,			1,			4,			2,				EPixelFormatFlag::RG | EPixelFormatFlag::TYPELESS	},
		{L"R16G16_FLOAT",				1,			1,			1,			4,			2,				EPixelFormatFlag::RG | EPixelFormatFlag::FLOAT		},
		{L"R16G16_UNORM",				1,			1,			1,			4,			2,				EPixelFormatFlag::RG | EPixelFormatFlag::UNORM		},
		{L"R16G16_UINT",				1,			1,			1,			4,			2,				EPixelFormatFlag::RG | EPixelFormatFlag::UINT		},
		{L"R16G16_SNORM",				1,			1,			1,			4,			2,				EPixelFormatFlag::RG | EPixelFormatFlag::SNORM		},
		{L"R16G16_SINT",				1,			1,			1,			4,			2,				EPixelFormatFlag::RG | EPixelFormatFlag::SINT		},
		{L"R32_TYPELESS",				1,			1,			1,			4,			1,				EPixelFormatFlag::R | EPixelFormatFlag::TYPELESS	},
		{L"D32_FLOAT",					1,			1,			1,			4,			1,				EPixelFormatFlag::D | EPixelFormatFlag::FLOAT		},
		{L"R32_FLOAT",					1,			1,			1,			4,			1,				EPixelFormatFlag::R | EPixelFormatFlag::FLOAT		},
		{L"R32_UINT",					1,			1,			1,			4,			1,				EPixelFormatFlag::R | EPixelFormatFlag::UINT		},
		{L"R32_SINT",					1,			1,			1,			4,			1,				EPixelFormatFlag::R | EPixelFormatFlag::SINT		},
		{L"R24G8_TYPELESS",				1,			1,			1,			4,			2,				EPixelFormatFlag::RG | EPixelFormatFlag::TYPELESS	},
		{L"D24_UNORM_S8_UINT",			1,			1,			1,			4,			2,				EPixelFormatFlag::DS | EPixelFormatFlag::SPECIAL	},
		{L"R24_UNORM_X8_TYPELESS",		1,			1,			1,			4,			1,				EPixelFormatFlag::R | EPixelFormatFlag::UNORM		},
		{L"X24_TYPELESS_G8_UINT",		1,			1,			1,			4,			1,				EPixelFormatFlag::G | EPixelFormatFlag::UINT		},
		{L"R8G8_TYPELESS",				1,			1,			1,			2,			2,				EPixelFormatFlag::RG | EPixelFormatFlag::TYPELESS	},
		{L"R8G8_UNORM",					1,			1,			1,			2,			2,				EPixelFormatFlag::RG | EPixelFormatFlag::UNORM		},
		{L"R8G8_UINT",					1,			1,			1,			2,			2,				EPixelFormatFlag::RG | EPixelFormatFlag::UINT		},
		{L"R8G8_SNORM",					1,			1,			1,			2,			2,				EPixelFormatFlag::RG | EPixelFormatFlag::SNORM		},
		{L"R8G8_SINT",					1,			1,			1,			2,			2,				EPixelFormatFlag::RG | EPixelFormatFlag::SINT		},
		{L"R16_TYPELESS",				1,			1,			1,			2,			1,				EPixelFormatFlag::R | EPixelFormatFlag::TYPELESS	},
		{L"R16_FLOAT",					1,			1,			1,			2,			1,				EPixelFormatFlag::R | EPixelFormatFlag::FLOAT		},
		{L"D16_UNORM",					1,			1,			1,			2,			1,				EPixelFormatFlag::D | EPixelFormatFlag::UNORM		},
		{L"R16_UNORM",					1,			1,			1,			2,			1,				EPixelFormatFlag::R | EPixelFormatFlag::UNORM		},
		{L"R16_UINT",					1,			1,			1,			2,			1,				EPixelFormatFlag::R | EPixelFormatFlag::UINT		},
		{L"R16_SNORM",					1,			1,			1,			2,			1,				EPixelFormatFlag::R | EPixelFormatFlag::SNORM		},
		{L"R16_SINT",					1,			1,			1,			2,			1,				EPixelFormatFlag::R | EPixelFormatFlag::SINT		},
		{L"R8_TYPELESS",				1,			1,			1,			1,			1,				EPixelFormatFlag::R | EPixelFormatFlag::TYPELESS	},
		{L"R8_UNORM",					1,			1,			1,			1,			1,				EPixelFormatFlag::R | EPixelFormatFlag::UNORM		},
		{L"R8_UINT",					1,			1,			1,			1,			1,				EPixelFormatFlag::R | EPixelFormatFlag::UINT		},
		{L"R8_SNORM",					1,			1,			1,			1,			1,				EPixelFormatFlag::R | EPixelFormatFlag::SNORM		},
		{L"R8_SINT",					1,			1,			1,			1,			1,				EPixelFormatFlag::R | EPixelFormatFlag::SINT		},
		{L"A8_UNORM",					1,			1,			1,			1,			1,				EPixelFormatFlag::A | EPixelFormatFlag::UNORM		},
		{L"R1_UNORM",					8,			1,			1,			1,			1,				EPixelFormatFlag::R | EPixelFormatFlag::UNORM		},
		{L"R9G9B9E5_SHAREDEXP",			1,			1,			1,			4,			3,				EPixelFormatFlag::RGB | EPixelFormatFlag::FLOAT	},
		{L"BC1_TYPELESS",				4,			4,			1,			8,			0,				EPixelFormatFlag::Error								},
		{L"BC1_UNORM",					4,			4,			1,			8,			0,				EPixelFormatFlag::Error								},
		{L"BC1_UNORM_SRGB",				4,			4,			1,			8,			0,				EPixelFormatFlag::Error								},
		{L"BC2_TYPELESS",				4,			4,			1,			16,			0,				EPixelFormatFlag::Error								},
		{L"BC2_UNORM",					4,			4,			1,			16,			0,				EPixelFormatFlag::Error								},
		{L"BC2_UNORM_SRGB",				4,			4,			1,			16,			0,				EPixelFormatFlag::Error								},
		{L"BC3_TYPELESS",				4,			4,			1,			16,			0,				EPixelFormatFlag::Error								},
		{L"BC3_UNORM",					4,			4,			1,			16,			0,				EPixelFormatFlag::Error								},
		{L"BC3_UNORM_SRGB",				4,			4,			1,			16,			0,				EPixelFormatFlag::Error								},
		{L"BC4_TYPELESS",				4,			4,			1,			8,			0,				EPixelFormatFlag::Error								},
		{L"BC4_UNORM",					4,			4,			1,			8,			0,				EPixelFormatFlag::Error								},
		{L"BC4_SNORM",					4,			4,			1,			8,			0,				EPixelFormatFlag::Error								},
		{L"BC5_TYPELESS",				4,			4,			1,			16,			0,				EPixelFormatFlag::Error								},
		{L"BC5_UNORM",					4,			4,			1,			16,			0,				EPixelFormatFlag::Error								},
		{L"BC5_SNORM",					4,			4,			1,			16,			0,				EPixelFormatFlag::Error								},
		{L"B5G6R5_UNORM",				1,			1,			1,			2,			0,				EPixelFormatFlag::Error								},
		{L"B5G5R5A1_UNORM",				1,			1,			1,			2,			0,				EPixelFormatFlag::Error								},
		{L"B8G8R8A8_UNORM",				1,			1,			1,			4,			0,				EPixelFormatFlag::Error								},
		{L"B8G8R8X8_UNORM",				1,			1,			1,			4,			0,				EPixelFormatFlag::Error								},
		{L"R10G10B10_XR_BIAS_A2_UNORM",	1,			1,			1,			4,			0,				EPixelFormatFlag::Error								},
		{L"B8G8R8A8_TYPELESS",			1,			1,			1,			4,			0,				EPixelFormatFlag::Error								},
		{L"B8G8R8A8_UNORM_SRGB",		1,			1,			1,			4,			0,				EPixelFormatFlag::Error								},
		{L"B8G8R8X8_TYPELESS",			1,			1,			1,			4,			0,				EPixelFormatFlag::Error								},
		{L"B8G8R8X8_UNORM_SRGB",		1,			1,			1,			4,			0,				EPixelFormatFlag::Error								},
		{L"BC6H_TYPELESS",				4,			4,			1,			16,			0,				EPixelFormatFlag::Error								},
		{L"BC6H_UF16",					4,			4,			1,			16,			0,				EPixelFormatFlag::Error								},
		{L"BC6H_SF16",					4,			4,			1,			16,			0,				EPixelFormatFlag::Error								},
		{L"BC7_TYPELESS",				4,			4,			1,			16,			0,				EPixelFormatFlag::Error								},
		{L"BC7_UNORM",					4,			4,			1,			16,			0,				EPixelFormatFlag::Error								},
		{L"BC7_UNORM_SRGB",				4,			4,			1,			16,			0,				EPixelFormatFlag::Error								},
	};

	return pixelFormatMetaMap[EnumToInt(_pixelFormat)];
}

uint32_t SPixelFormatMeta::GetPixelRowPitch(ERDIPixelFormat _pixelFormat, uint32_t _width, uint32_t _mipSlice)
{
	if (_width == 0)
		return 0;

	_width = std::max(1u, _width >> _mipSlice);

	SPixelFormatMeta meta = GetPixelFormatMeta(_pixelFormat);

	uint32_t xBlockCount = Math::CalcBlockCount<uint32_t>(_width, meta.mBlockSizeX);

	return xBlockCount * meta.mBlockBytes;
}

uint32_t SPixelFormatMeta::GetPixelScanlines(ERDIPixelFormat _pixelFormat, uint32_t _height, uint32_t _mipSlice)
{
	if (_height == 0)
		return 0;

	_height = std::max(1u, _height >> _mipSlice);

	SPixelFormatMeta meta = GetPixelFormatMeta(_pixelFormat);

	return Math::CalcBlockCount<uint32_t>(_height, meta.mBlockSizeY);
}

uint32_t SPixelFormatMeta::GetPixelSlicePitch(ERDIPixelFormat _pixelFormat, uint32_t _width, uint32_t _height, uint32_t _mipSlice)
{
	return GetPixelScanlines(_pixelFormat, _height, _mipSlice) * GetPixelRowPitch(_pixelFormat, _width, _mipSlice);
}

uint32_t SPixelFormatMeta::GetPixelSlicePitch(ERDIPixelFormat _pixelFormat, uint32_t _width, uint32_t _height, uint32_t _depth, uint32_t _mipSlice)
{
	return GetPixelScanlines(_pixelFormat, _height, _mipSlice) * GetPixelRowPitch(_pixelFormat, _width, _mipSlice) * _depth;
}

uint32_t SPixelFormatMeta::GetPixelFootprint(ERDIPixelFormat _pixelFormat, uint32_t _width, uint32_t _height, uint32_t _mipCount)
{
	uint32_t footprint = 0;

	for (uint32_t i = 0; i != _mipCount; ++i)
		footprint += GetPixelSlicePitch(_pixelFormat, _width, _height, i);

	return footprint;
}

uint32_t SPixelFormatMeta::GetPixelFootprint(ERDIPixelFormat _pixelFormat, uint32_t _width, uint32_t _height, uint32_t _mipCount, uint32_t _arraySize)
{
	return GetPixelFootprint(_pixelFormat, _width, _height, _mipCount) * _arraySize;
}
