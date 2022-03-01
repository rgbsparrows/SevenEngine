#pragma once

#include "Core/Util/TemplateUtil.h"
#include "Core/Util/UtilMacros.h"

#include <string_view>

enum class ERDIPixelFormat
{
	UNKNOWN,
	R32G32B32A32_TYPELESS,
	R32G32B32A32_FLOAT,
	R32G32B32A32_UINT,
	R32G32B32A32_SINT,
	R32G32B32_TYPELESS,
	R32G32B32_FLOAT,
	R32G32B32_UINT,
	R32G32B32_SINT,
	R16G16B16A16_TYPELESS,
	R16G16B16A16_FLOAT,
	R16G16B16A16_UNORM,
	R16G16B16A16_UINT,
	R16G16B16A16_SNORM,
	R16G16B16A16_SINT,
	R32G32_TYPELESS,
	R32G32_FLOAT,
	R32G32_UINT,
	R32G32_SINT,
	R32G8X24_TYPELESS,
	D32_FLOAT_S8X24_UINT,
	R32_FLOAT_X8X24_TYPELESS,
	X32_TYPELESS_G8X24_UINT,
	R10G10B10A2_TYPELESS,
	R10G10B10A2_UNORM,
	R10G10B10A2_UINT,
	R11G11B10_FLOAT,
	R8G8B8A8_TYPELESS,
	R8G8B8A8_UNORM,
	R8G8B8A8_UNORM_SRGB,
	R8G8B8A8_UINT,
	R8G8B8A8_SNORM,
	R8G8B8A8_SINT,
	R16G16_TYPELESS,
	R16G16_FLOAT,
	R16G16_UNORM,
	R16G16_UINT,
	R16G16_SNORM,
	R16G16_SINT,
	R32_TYPELESS,
	D32_FLOAT,
	R32_FLOAT,
	R32_UINT,
	R32_SINT,
	R24G8_TYPELESS,
	D24_UNORM_S8_UINT,
	R24_UNORM_X8_TYPELESS,
	X24_TYPELESS_G8_UINT,
	R8G8_TYPELESS,
	R8G8_UNORM,
	R8G8_UINT,
	R8G8_SNORM,
	R8G8_SINT,
	R16_TYPELESS,
	R16_FLOAT,
	D16_UNORM,
	R16_UNORM,
	R16_UINT,
	R16_SNORM,
	R16_SINT,
	R8_TYPELESS,
	R8_UNORM,
	R8_UINT,
	R8_SNORM,
	R8_SINT,
	A8_UNORM,
	R1_UNORM,
	R9G9B9E5_SHAREDEXP,
	BC1_TYPELESS,
	BC1_UNORM,
	BC1_UNORM_SRGB,
	BC2_TYPELESS,
	BC2_UNORM,
	BC2_UNORM_SRGB,
	BC3_TYPELESS,
	BC3_UNORM,
	BC3_UNORM_SRGB,
	BC4_TYPELESS,
	BC4_UNORM,
	BC4_SNORM,
	BC5_TYPELESS,
	BC5_UNORM,
	BC5_SNORM,
	B5G6R5_UNORM,
	B5G5R5A1_UNORM,
	B8G8R8A8_UNORM,
	B8G8R8X8_UNORM,
	R10G10B10_XR_BIAS_A2_UNORM,
	B8G8R8A8_TYPELESS,
	B8G8R8A8_UNORM_SRGB,
	B8G8R8X8_TYPELESS,
	B8G8R8X8_UNORM_SRGB,
	BC6H_TYPELESS,
	BC6H_UF16,
	BC6H_SF16,
	BC7_TYPELESS,
	BC7_UNORM,
	BC7_UNORM_SRGB,
	
	Num,
	Error = TErrorEnumValue<ERDIPixelFormat>
};

enum class EPixelFormatFlag
{
	None = 0,

	R = 1 << 0,
	G = 1 << 1,
	B = 1 << 2,
	A = 1 << 3,
	D = 1 << 4,
	S = 1 << 5,

	RG = R | G,
	RGB = R | G | B,
	RGBA = RGB | A,
	DS = D | S,

	TYPELESS = 1 << 6,
	UNORM = 2 << 6,
	SNORM = 3 << 6,
	UINT = 4 << 6,
	SINT = 5 << 6,
	FLOAT = 6 << 6,
	SPECIAL = 7 << 6,

	Compressed = 1 << 9,
	SRGB = 1 << 10,

	Error = TErrorEnumValue<EPixelFormatFlag>
};
REGIST_ENUM_FLAG(EPixelFormatFlag)

struct SPixelFormatMeta
{
	std::wstring_view mPixelFormatName;
	uint16_t mBlockSizeX = 0;
	uint16_t mBlockSizeY = 0;
	uint16_t mBlockSizeZ = 0;
	uint16_t mBlockBytes = 0;
	uint16_t mNumComponents = 0;
	EPixelFormatFlag mFlag = EPixelFormatFlag::None;

	static SPixelFormatMeta GetPixelFormatMeta(ERDIPixelFormat _pixelFormat);

	static uint32_t GetPixelRowPitch(ERDIPixelFormat _pixelFormat, uint32_t _width, uint32_t _mipSlice);
	static uint32_t GetPixelScanlines(ERDIPixelFormat _pixelFormat, uint32_t _height, uint32_t _mipSlice);

	static uint32_t GetPixelSlicePitch(ERDIPixelFormat _pixelFormat, uint32_t _width, uint32_t _height, uint32_t _mipSlice);
	static uint32_t GetPixelSlicePitch(ERDIPixelFormat _pixelFormat, uint32_t _width, uint32_t _height, uint32_t _depth, uint32_t _mipSlice);

	static uint32_t GetPixelFootprint(ERDIPixelFormat _pixelFormat, uint32_t _width, uint32_t _height, uint32_t _mipCount);
	static uint32_t GetPixelFootprint(ERDIPixelFormat _pixelFormat, uint32_t _width, uint32_t _height, uint32_t _mipCount, uint32_t _arraySize);
};
