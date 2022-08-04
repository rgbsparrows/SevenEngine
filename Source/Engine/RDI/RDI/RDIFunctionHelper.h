#pragma once

#include "Core/Math/Type.h"
#include "RDI/RDIPixelFormat.h"
#include "Core/Util/UtilMacros.h"
#include "Core/Container/Blob.h"
#include "Core/Util/TemplateUtil.h"
#include "Core/Misc/windowsEx.h"

#include <string_view>
#include <filesystem>

__interface IRDIRootSignature;
__interface IRDIInputLayout;

__interface IRDIVertexShader;
__interface IRDIHullShader;
__interface IRDIDomainShader;
__interface IRDIGeometryShader;
__interface IRDIPixelShader;
__interface IRDIComputeShader;

class SBlob;

#pragma region RDICommon

struct SRDIFutureSupport
{

};

struct SRDIErrorInfo
{
	std::wstring mErrorString;
	std::vector<std::wstring_view> mParsedErrorString;
};

#pragma endregion

#pragma region RDICommandList

enum class ERDICommandListType
{
	Direct,
	Compute,

	Num,
};

#pragma endregion

#pragma region RDIAdapter

struct SRDIAdapterDesc
{
	std::wstring_view mDescription;
	uint64_t mDedicatedVideoMemory = 0;
	uint64_t mDedicatedSystemMemory = 0;
	uint64_t mSharedSystemMemory = 0;
};

#pragma endregion

#pragma region RDIOutput

struct SRDIOutputDesc
{
	std::wstring_view mDeviceName;
	Math::SIntRect mDesktopCoordinates;
	bool mAttachedToDesktop = false;
};

#pragma endregion

#pragma region RDISwapChain

enum class ERDIScalingMode
{
	Unspecified,
	Centered,
	Stretched,

	Error = TErrorEnumValue<ERDIScalingMode>
};

struct SRDISwapChainDesc
{
	uint32_t mWidth = 0;
	uint32_t mHeight = 0;
	Math::SUIntFraction mRefreshRate;
	ERDIPixelFormat mPixelFormat = ERDIPixelFormat::UNKNOWN;
	ERDIScalingMode mScalingMode = ERDIScalingMode::Stretched;
	uint32_t mBufferCount = 0;
	HWND mOutputWindow = nullptr;
	bool mIsWindowed = true;
};

#pragma endregion

#pragma region resource

enum class ERDIHeapType
{
	Default,
	Upload,
	ReadBack,
	Custom,

	Num,
	Error = TErrorEnumValue<ERDIHeapType>
};

enum class ERDIResourceUsage
{
	None = 0,
	IndexBuffer = 1 << 0,
	VertexBuffer = 1 << 1,
	RenderTarget = 1 << 2,
	DepthStencil = 1 << 3,
	ConstantBuffer = 1 << 4,
	ShaderResource = 1 << 5,
	UnorderAccess = 1 << 6,

	Error = TErrorEnumValue<ERDIResourceUsage>
};
REGIST_ENUM_FLAG(ERDIResourceUsage)

enum class ERDIResourceState
{
	Common,
	VertexAndConstantBuffer,
	IndexBuffer,
	RenderTarget,
	UnorderedAccess,
	DepthWrite,
	DepthRead,
	NonPixelShaderResource,
	PixelShaderResource,
	StreamOut,
	IndirectArgument,
	CopyDest,
	CopySource,
	ResolveDest,
	ResolveSource,
	RayTracingAccelerationStructure,
	GenericRead,
	Present,
	Predication,

	Num,
	Error = TErrorEnumValue<ERDIResourceState>
};

enum class ERDITextureCubeFace
{
	PositiveX,
	NegativeX,
	PositiveY,
	NegativeY,
	PositiveZ,
	NegativeZ,
	FaceCount = 6,

	Error = TErrorEnumValue<ERDITextureCubeFace>
};

enum class ERDIFilter
{
	MIP_POINT		= 0x000,
	MIP_LINER		= 0x001,
	MAG_POINT		= 0x000,
	MAG_LINER		= 0x004,
	MIN_POINT		= 0x000,
	MIN_LINER		= 0x010,
	ANISOTROPIC		= 0X055,
	COMPARISON		= 0X080,
	MINIMUM			= 0x100,
	MAXIUM			= 0X180,

	Error = TErrorEnumValue<ERDIFilter>
};
REGIST_ENUM_FLAG(ERDIFilter)

enum class ERDIAddressMode
{
	WRAP			= 1,
	MIRROR			= 2,
	CLAMP			= 3,
	BORDER			= 4,
	MIRROR_ONCE		= 5
};

enum class ERDIComparisonFunc;

struct SRDIBufferResourceDesc
{
	ERDIHeapType mHeapType = ERDIHeapType::Default;
	ERDIResourceUsage mResourceUsage = ERDIResourceUsage::None;
	uint64_t mBufferSize = 0;
	//只在Buffer用作IndexBuffer/VertexBuffer时有意义
	uint32_t mElementStride = 0;
};

struct SRDITexture1DResourceDesc
{
	ERDIHeapType mHeapType = ERDIHeapType::Default;
	ERDIResourceUsage mResourceUsage = ERDIResourceUsage::None;
	ERDIPixelFormat mPixelFormat = ERDIPixelFormat::UNKNOWN;
	ERDIPixelFormat mRtvPixelFormat = ERDIPixelFormat::UNKNOWN;
	ERDIPixelFormat mDsvPixelFormat = ERDIPixelFormat::UNKNOWN;
	ERDIPixelFormat mSrvPixelFormat = ERDIPixelFormat::UNKNOWN;
	ERDIPixelFormat mUavPixelFormat = ERDIPixelFormat::UNKNOWN;

	uint16_t mMipCount = 0;

	uint32_t mSizeX = 0;
};

struct SRDITexture1DArrayResourceDesc
{
	ERDIHeapType mHeapType = ERDIHeapType::Default;
	ERDIResourceUsage mResourceUsage = ERDIResourceUsage::None;
	ERDIPixelFormat mPixelFormat = ERDIPixelFormat::UNKNOWN;
	ERDIPixelFormat mRtvPixelFormat = ERDIPixelFormat::UNKNOWN;
	ERDIPixelFormat mDsvPixelFormat = ERDIPixelFormat::UNKNOWN;
	ERDIPixelFormat mSrvPixelFormat = ERDIPixelFormat::UNKNOWN;
	ERDIPixelFormat mUavPixelFormat = ERDIPixelFormat::UNKNOWN;

	uint16_t mMipCount = 0;
	uint16_t mArraySize = 0;

	uint32_t mSizeX = 0;
};

struct SRDITexture2DResourceDesc
{
	ERDIHeapType mHeapType = ERDIHeapType::Default;
	ERDIResourceUsage mResourceUsage = ERDIResourceUsage::None;
	ERDIPixelFormat mPixelFormat = ERDIPixelFormat::UNKNOWN;
	ERDIPixelFormat mRtvPixelFormat = ERDIPixelFormat::UNKNOWN;
	ERDIPixelFormat mDsvPixelFormat = ERDIPixelFormat::UNKNOWN;
	ERDIPixelFormat mSrvPixelFormat = ERDIPixelFormat::UNKNOWN;
	ERDIPixelFormat mUavPixelFormat = ERDIPixelFormat::UNKNOWN;

	uint16_t mMipCount = 1;

	uint32_t mSizeX = 0;
	uint32_t mSizeY = 0;

	bool operator ==(const SRDITexture2DResourceDesc&) const = default;
	bool operator !=(const SRDITexture2DResourceDesc&) const = default;
};

struct SRDITexture2DArrayResourceDesc
{
	ERDIHeapType mHeapType = ERDIHeapType::Default;
	ERDIResourceUsage mResourceUsage = ERDIResourceUsage::None;
	ERDIPixelFormat mPixelFormat = ERDIPixelFormat::UNKNOWN;
	ERDIPixelFormat mRtvPixelFormat = ERDIPixelFormat::UNKNOWN;
	ERDIPixelFormat mDsvPixelFormat = ERDIPixelFormat::UNKNOWN;
	ERDIPixelFormat mSrvPixelFormat = ERDIPixelFormat::UNKNOWN;
	ERDIPixelFormat mUavPixelFormat = ERDIPixelFormat::UNKNOWN;

	uint16_t mMipCount = 0;
	uint16_t mArraySize = 0;

	uint32_t mSizeX = 0;
	uint32_t mSizeY = 0;
};

struct SRDITexture3DResourceDesc
{
	ERDIHeapType mHeapType = ERDIHeapType::Default;
	ERDIResourceUsage mResourceUsage = ERDIResourceUsage::None;
	ERDIPixelFormat mPixelFormat = ERDIPixelFormat::UNKNOWN;
	ERDIPixelFormat mRtvPixelFormat = ERDIPixelFormat::UNKNOWN;
	ERDIPixelFormat mDsvPixelFormat = ERDIPixelFormat::UNKNOWN;
	ERDIPixelFormat mSrvPixelFormat = ERDIPixelFormat::UNKNOWN;
	ERDIPixelFormat mUavPixelFormat = ERDIPixelFormat::UNKNOWN;

	uint16_t mMipCount = 0;

	uint32_t mSizeX = 0;
	uint32_t mSizeY = 0;
	uint32_t mSizeZ = 0;
};

struct SRDITextureCubeResourceDesc
{
	ERDIHeapType mHeapType = ERDIHeapType::Default;
	ERDIResourceUsage mResourceUsage = ERDIResourceUsage::None;
	ERDIPixelFormat mPixelFormat = ERDIPixelFormat::UNKNOWN;
	ERDIPixelFormat mRtvPixelFormat = ERDIPixelFormat::UNKNOWN;
	ERDIPixelFormat mDsvPixelFormat = ERDIPixelFormat::UNKNOWN;
	ERDIPixelFormat mSrvPixelFormat = ERDIPixelFormat::UNKNOWN;
	ERDIPixelFormat mUavPixelFormat = ERDIPixelFormat::UNKNOWN;

	uint16_t mMipCount = 0;

	uint32_t mSizeXY = 0;
};

struct SRDITextureCubeArrayResourceDesc
{
	ERDIHeapType mHeapType = ERDIHeapType::Default;
	ERDIResourceUsage mResourceUsage = ERDIResourceUsage::None;
	ERDIPixelFormat mPixelFormat = ERDIPixelFormat::UNKNOWN;
	ERDIPixelFormat mRtvPixelFormat = ERDIPixelFormat::UNKNOWN;
	ERDIPixelFormat mDsvPixelFormat = ERDIPixelFormat::UNKNOWN;
	ERDIPixelFormat mSrvPixelFormat = ERDIPixelFormat::UNKNOWN;
	ERDIPixelFormat mUavPixelFormat = ERDIPixelFormat::UNKNOWN;

	uint16_t mMipCount = 0;
	uint16_t mArraySize = 0;

	uint32_t mSizeXY = 0;
};

struct SRDISamplerResourceDesc
{
	ERDIFilter mFilter;
	ERDIAddressMode mAddressU;
	ERDIAddressMode mAddressV;
	ERDIAddressMode mAddressW;
	float mMipLODBias;
	uint32_t mMaxAnisotropy;
	ERDIComparisonFunc mComparisonFunc;
	Math::SFColor mBorderColor;
	float mMinLod;
	float mMaxLod;
};

#pragma endregion

#pragma region shader compile

//尽可能的让信息储存在Shader/材质文件本身当中，以排除掉由程序额外指定参数对代码阅读所造成的困扰，所以这里剔除掉宏参数，只保留宏本身
struct SRDIShaderMacro
{
	std::vector<std::wstring_view> mDefinedMacro;
};

#pragma endregion

#pragma region root signature

enum class ERDIRootParameterType
{
	CBV,
	DescriptorTable,
	SamplerTable,

	Error = TErrorEnumValue<ERDIRootParameterType>
};

enum class ERDIShaderVisibility
{
	ALL_VISABLE,
	VERTEX_VISABLE,
	HULL_VISABLE,
	DOMAIN_VISABLE,
	GEOMETRY_VISABLE,
	PIXEL_VISABLE,

	Error = TErrorEnumValue<ERDIShaderVisibility>
};

struct SRDICbvRootParameter
{
	uint32_t mCbvShaderRegister;
};

struct SRDIDescriptorTableRootParameter
{
	uint32_t mSrvStartShaderRegister;
	uint32_t mSrvNumDescriptors;

	uint32_t mUavStartShaderRegister;
	uint32_t mUavNumDescriptors;
};

struct SRDISamplerTableRootParameter
{
	uint32_t mSamplerStartShaderRegister;
	uint32_t mSamplerNumDescriptors;
};

struct SRDIRootParameter
{
	ERDIRootParameterType mType = ERDIRootParameterType::Error;
	ERDIShaderVisibility mShaderVisibility = ERDIShaderVisibility::ALL_VISABLE;
	uint32_t mRegisterSpace = 0;

	union
	{
		SRDICbvRootParameter mCbvRootParameter;
		SRDIDescriptorTableRootParameter mDescriptorTableRootParameter;
		SRDISamplerTableRootParameter mSamplerTableRootParameter;
	};
};

enum class ERDIStaticBorderColor
{
	TRANSPARENT_BLACK,
	OPAQUE_BLACK,
	OPAQUE_WHITE,

	Error = TErrorEnumValue<ERDIStaticBorderColor>
};

struct SRDIStaticSamplerDesc
{
	ERDIFilter mFilter;
	ERDIAddressMode mAddressU;
	ERDIAddressMode mAddressV;
	ERDIAddressMode mAddressW;
	float mMipLODBias;
	uint32_t mMaxAnisotropy;
	ERDIComparisonFunc mComparisonFunc;
	ERDIStaticBorderColor mBorderColor;
	float mMinLod;
	float mMaxLod;
	uint32_t mShaderRegister;
	uint32_t mRegisterSpace;
	ERDIShaderVisibility mShaderVisibility;
};

struct SRDIRootSignatureDesc
{
	std::vector<SRDIRootParameter> mRootParameters;
	std::vector<SRDIStaticSamplerDesc> mStaticSamplerDescs;
};

#pragma endregion

#pragma region input layout

struct SRDIVertexInputElememt
{
	std::wstring_view mSemanticName;
	ERDIPixelFormat mFormat;
	uint32_t mAlignedByteOffset;
};

struct SRDIVertexInputLayoutDesc
{
	std::vector<SRDIVertexInputElememt> mInputElements;
};

#pragma endregion

#pragma region rasterization pipeline

enum class ERDIBlendFactory
{
	ZERO,
	ONE,
	SRC_COLOR,
	INV_SRC_COLOR,
	SRC_ALPHA,
	INV_SRC_ALPHA,
	DEST_ALPHA,
	INV_DEST_ALPHA,
	DEST_COLOR,
	INV_DEST_COLOR,
	SRC_ALPHA_SAT,
	BLEND_FACTOR,
	INV_BLEND_FACTOR,
	SRC1_COLOR,
	INV_SRC1_COLOR,
	SRC1_ALPHA,
	INV_SRC1_ALPHA,

	Error = TErrorEnumValue<ERDIBlendFactory>
};

enum class ERDIBlendOperator
{
	ADD,
	SUBTRACT,
	REV_SUBTRACT,
	MIN,
	MAX,

	Error = TErrorEnumValue<ERDIBlendOperator>
};

enum class ERDILogicOperator
{
	CLEAR,
	SET,
	COPY,
	COPY_INVERTED,
	NOOP,
	INVERT,
	AND,
	NAND,
	OR,
	NOR,
	XOR,
	EQUIV,
	AND_REVERSE,
	AND_INVERTED,
	OR_REVERSE,
	OR_INVERTED,

	Error = TErrorEnumValue<ERDILogicOperator>
};

enum class ERDIColorWriteMask
{
	RED = 1 << 0,
	GREEN = 1 << 1,
	BLUE = 1 << 2,
	ALPHA = 1 << 3,

	ALL = RED | GREEN | BLUE | ALPHA,

	Error = TErrorEnumValue<ERDIColorWriteMask>
};
REGIST_ENUM_FLAG(ERDIColorWriteMask)

struct SRDIBlendState
{
	bool mBlendEnable = false;
	bool mLogicOpEnable = false;

	ERDIBlendFactory mSrcBlend = ERDIBlendFactory::SRC_ALPHA;
	ERDIBlendFactory mDestBlend = ERDIBlendFactory::INV_SRC_ALPHA;
	ERDIBlendOperator mBlendOperator = ERDIBlendOperator::ADD;

	ERDIBlendFactory mSrcAlphaBlend = ERDIBlendFactory::ONE;
	ERDIBlendFactory mDestAlphaBlend = ERDIBlendFactory::ZERO;
	ERDIBlendOperator mBlendAlphaOperator = ERDIBlendOperator::ADD;

	ERDILogicOperator mLogicOperator = ERDILogicOperator::SET;

	ERDIColorWriteMask mColorWriteMask = ERDIColorWriteMask::ALL;
};

enum class ERDIFillMode
{
	WIREFRAME,
	SOLID,

	Error = TErrorEnumValue<ERDIFillMode>
};

enum class ERDICullMode
{
	None,
	FRONT_FACE,
	BACK_FACE,

	Error = TErrorEnumValue<ERDICullMode>
};

enum class ERDIConservativeRasterizationMode
{
	OFF,
	ON,

	Error = TErrorEnumValue<ERDIConservativeRasterizationMode>
};

struct SRDIRasterizationState
{
	ERDIFillMode mFillMode = ERDIFillMode::SOLID;
	ERDICullMode mCullMode = ERDICullMode::BACK_FACE;
	uint32_t mDepthBias = 0;
	float mDepthBiasClamp = 0.f;
	float mSlopeScaledDepthBias = 0.f;
	bool mDepthClipEnable = false;
	ERDIConservativeRasterizationMode mConservativeRasterizationMode = ERDIConservativeRasterizationMode::OFF;
};

enum class ERDIComparisonFunc
{
	NEVER,
	LESS,
	EQUAL,
	LESS_EQUAL,
	GREATER,
	NOT_EQUAL,
	GREATER_EQUAL,
	ALWAYS,

	Error = TErrorEnumValue<ERDIComparisonFunc>
};

enum class ERDIStencilOperator
{
	KEEP,
	ZERO,
	REPLACE,
	INCR_SAT,
	DECR_SAT,
	INVERT,
	INCR,
	DECR,

	Error = TErrorEnumValue<ERDIStencilOperator>
};

struct SRDIDepthStencilOperatorDesc
{
	ERDIStencilOperator mStencilFailOp = ERDIStencilOperator::KEEP;
	ERDIStencilOperator mStencilDepthFailOp = ERDIStencilOperator::KEEP;
	ERDIStencilOperator mStencilPassOp = ERDIStencilOperator::KEEP;
	ERDIComparisonFunc mStencilCompareFunc = ERDIComparisonFunc::ALWAYS;
};

struct SRDIDepthStencilState
{
	bool mDepthTestEnable = true;
	bool mDepthWriteEnable = true;
	ERDIComparisonFunc mDepthCompareFunc = ERDIComparisonFunc::LESS_EQUAL;
	bool mStencilTestEnable = false;
	uint8_t mStencilReadMask = 0xff;
	uint8_t mStencilWriteMask = 0xff;
	SRDIDepthStencilOperatorDesc mFrontFaceDepthStencilOperator;
	SRDIDepthStencilOperatorDesc mBackFaceDepthStencilOperator;
};

enum class ERDIPrimitiveTopologyType
{
	UNDEFINED,
	POINT,
	LINE,
	TRIANGLE,
	PATCH,

	Error = TErrorEnumValue<ERDIPrimitiveTopologyType>
};

struct SRDIGraphicsPipelineState
{
	IRDIRootSignature* mRootSignature = nullptr;
	IRDIVertexShader* mVertexShader = nullptr;
	IRDIHullShader* mHullShader = nullptr;
	IRDIDomainShader* mDomainShader = nullptr;
	IRDIGeometryShader* mGeometryShader = nullptr;
	IRDIPixelShader* mPixelShader = nullptr;
	IRDIInputLayout* mInputLayout = nullptr;
	ERDIPrimitiveTopologyType mPrimitiveTopologyType = ERDIPrimitiveTopologyType::TRIANGLE;
	SRDIBlendState mBlendState;
	SRDIRasterizationState mRasterizationState;
	SRDIDepthStencilState mDepthStencilState;
	uint32_t mRenderTargetCount = 1;
	ERDIPixelFormat mRenderTargetFormat[8] = {};
	ERDIPixelFormat mDepthStencilFormat = ERDIPixelFormat::D24_UNORM_S8_UINT;
	SBufferView mCachedPSO;
};

enum class ERDIPrimitiveTopology
{
	UNDEFINED,
	POINTLIST,
	LINELIST,
	LINESTRIP,
	TRIANGLELIST,
	TRIANGLESTRIP,
	LINELIST_ADJ,
	LINESTRIP_ADJ,
	TRIANGLELIST_ADJ,
	TRIANGLESTRIP_ADJ,
	CONTROL_POINT_1_PATCHLIST,
	CONTROL_POINT_2_PATCHLIST,
	CONTROL_POINT_3_PATCHLIST,
	CONTROL_POINT_4_PATCHLIST,
	CONTROL_POINT_5_PATCHLIST,
	CONTROL_POINT_6_PATCHLIST,
	CONTROL_POINT_7_PATCHLIST,
	CONTROL_POINT_8_PATCHLIST,
	CONTROL_POINT_9_PATCHLIST,
	CONTROL_POINT_10_PATCHLIST,
	CONTROL_POINT_11_PATCHLIST,
	CONTROL_POINT_12_PATCHLIST,
	CONTROL_POINT_13_PATCHLIST,
	CONTROL_POINT_14_PATCHLIST,
	CONTROL_POINT_15_PATCHLIST,
	CONTROL_POINT_16_PATCHLIST,
	CONTROL_POINT_17_PATCHLIST,
	CONTROL_POINT_18_PATCHLIST,
	CONTROL_POINT_19_PATCHLIST,
	CONTROL_POINT_20_PATCHLIST,
	CONTROL_POINT_21_PATCHLIST,
	CONTROL_POINT_22_PATCHLIST,
	CONTROL_POINT_23_PATCHLIST,
	CONTROL_POINT_24_PATCHLIST,
	CONTROL_POINT_25_PATCHLIST,
	CONTROL_POINT_26_PATCHLIST,
	CONTROL_POINT_27_PATCHLIST,
	CONTROL_POINT_28_PATCHLIST,
	CONTROL_POINT_29_PATCHLIST,
	CONTROL_POINT_30_PATCHLIST,
	CONTROL_POINT_31_PATCHLIST,
	CONTROL_POINT_32_PATCHLIST,

	Num,
	Error = TErrorEnumValue<ERDIPrimitiveTopology>
};

#pragma endregion

#pragma region compute pipeline

struct SRDIComputePipelineState
{
	IRDIRootSignature* mRootSignature = nullptr;
	IRDIComputeShader* mComputeShader = nullptr;
	SBufferView mCachedPSO;
};

#pragma endregion

#pragma region command list

enum class ERDIClearFlag
{
	Depth = 1 << 0,
	Stencil = 1 << 1,

	Error = TErrorEnumValue<ERDIClearFlag>
};

REGIST_ENUM_FLAG(ERDIClearFlag)

#pragma endregion
