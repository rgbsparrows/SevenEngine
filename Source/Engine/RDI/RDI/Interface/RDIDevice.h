#pragma once

#include <stdint.h>

class SBufferView;

struct SRDIBufferResourceDesc;
struct SRDITexture1DResourceDesc;
struct SRDITexture1DArrayResourceDesc;
struct SRDITexture2DResourceDesc;
struct SRDITexture2DArrayResourceDesc;
struct SRDITexture3DResourceDesc;
struct SRDITextureCubeResourceDesc;
struct SRDITextureCubeArrayResourceDesc;
struct SRDISamplerResourceDesc;

struct SRDISwapChainDesc;
struct SRDIVertexInputLayoutDesc;
struct SRDIRootSignatureDesc;
struct SRDIGraphicsPipelineState;
struct SRDIComputePipelineState;

struct SRDIShaderMacro;
struct SRDIAdditionalInclude;
struct SRDIErrorInfo;

__interface IRDIAdapter;
__interface IRDISwapChain;
__interface IRDICommandList;
__interface IRDIInputLayout;
__interface IRDICommandQueue;
__interface IRDIRootSignature;
__interface IRDIDescriptorHeapRange;
__interface IRDISamplerHeapRange;
__interface IRDIGraphicsPipelineState;
__interface IRDIComputePipelineState;

__interface IRDIBuffer;
__interface IRDITexture1D;
__interface IRDITexture1DArray;
__interface IRDITexture2D;
__interface IRDITexture2DArray;
__interface IRDITexture3D;
__interface IRDITextureCube;
__interface IRDITextureCubeArray;
__interface IRDISampler;

__interface IRDIVertexShader;
__interface IRDIHullShader;
__interface IRDIDomainShader;
__interface IRDIGeometryShader;
__interface IRDIPixelShader;
__interface IRDIComputeShader;

__interface IRDIDevice
{
	IRDIAdapter* GetAdapter() noexcept;
	IRDICommandQueue* GetCommandQueue() noexcept;
	IRDICommandList* GetCommandList(uint16_t _commandListIndex) noexcept;

	IRDISwapChain* CreateSwapChain(const SRDISwapChainDesc* _swapChainDesc) noexcept;

	void EnsureCommandListCount(size_t _commandListCount) noexcept;
	void ResetCommandListAlocator(size_t _commandAllocatorIndex) noexcept;
	void SetCurrentCommandListAllocator(size_t _commandAllocatorIndex) noexcept;

	IRDIInputLayout* CreateInputLayout(const SRDIVertexInputLayoutDesc* _desc) noexcept;

	IRDIRootSignature* CreateRootSignature(const SRDIRootSignatureDesc* _desc, SRDIErrorInfo* _rootSignatureError) noexcept;
	IRDIRootSignature* CreateRootSignature(const SBufferView _serializedRootSignatureBlob) noexcept;

	IRDIGraphicsPipelineState* CreateGraphicsPipelineState(const SRDIGraphicsPipelineState* _desc) noexcept;
	IRDIComputePipelineState* CreateComputePipelineState(const SRDIComputePipelineState* _desc) noexcept;

	IRDIBuffer* CreateBuffer(const SRDIBufferResourceDesc* _desc) noexcept;
	IRDITexture1D* CreateTexture1D(const SRDITexture1DResourceDesc* _desc) noexcept;
	IRDITexture1DArray* CreateTexture1DArray(const SRDITexture1DArrayResourceDesc* _desc) noexcept;
	IRDITexture2D* CreateTexture2D(const SRDITexture2DResourceDesc* _desc) noexcept;
	IRDITexture2DArray* CreateTexture2DArray(const SRDITexture2DArrayResourceDesc* _desc) noexcept;
	IRDITexture3D* CreateTexture3D(const SRDITexture3DResourceDesc* _desc) noexcept;
	IRDITextureCube* CreateTextureCube(const SRDITextureCubeResourceDesc* _desc) noexcept;
	IRDITextureCubeArray* CreateTextureCubeArray(const SRDITextureCubeArrayResourceDesc* _desc) noexcept;
	IRDISampler* CreateSampler(const SRDISamplerResourceDesc* _desc) noexcept;

	IRDIDescriptorHeapRange* CreateDescriptorRange(uint16_t _srvCount, uint16_t _uavCount) noexcept;
	IRDISamplerHeapRange* CreateDescriptorRange(uint16_t _samplerCount) noexcept;

	IRDIVertexShader* CreateVertexShader(SBufferView _hlslShader, const SRDIShaderMacro* _shaderMacro, SRDIErrorInfo* _shaderCompileError) noexcept;
	IRDIHullShader* CreateHullShader(SBufferView _hlslShader, const SRDIShaderMacro* _shaderMacro, SRDIErrorInfo* _shaderCompileError) noexcept;
	IRDIDomainShader* CreateDomainShader(SBufferView _hlslShader, const SRDIShaderMacro* _shaderMacro, SRDIErrorInfo* _shaderCompileError) noexcept;
	IRDIGeometryShader* CreateGeometryShader(SBufferView _hlslShader, const SRDIShaderMacro* _shaderMacro, SRDIErrorInfo* _shaderCompileError) noexcept;
	IRDIPixelShader* CreatePixelShader(SBufferView _hlslShader, const SRDIShaderMacro* _shaderMacro, SRDIErrorInfo* _shaderCompileError) noexcept;
	IRDIComputeShader* CreateComputeShader(SBufferView _hlslShader, const SRDIShaderMacro* _shaderMacro, SRDIErrorInfo* _shaderCompileError) noexcept;

	IRDIVertexShader* CreateVertexShader(SBufferView _compiledShader) noexcept;
	IRDIHullShader* CreateHullShader(SBufferView _compiledShader) noexcept;
	IRDIDomainShader* CreateDomainShader(SBufferView _compiledShader) noexcept;
	IRDIGeometryShader* CreateGeometryShader(SBufferView _compiledShader) noexcept;
	IRDIPixelShader* CreatePixelShader(SBufferView _compiledShader) noexcept;
	IRDIComputeShader* CreateComputeShader(SBufferView _compiledShader) noexcept;
};
