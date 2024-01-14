#pragma once

#include "RDI/Interface/RDIFactory.h"
#include "D3D/D3D12/D3D12Device.h"
#include "D3D/D3D12/D3D12Adapter.h"

#include "Core/Misc/PreWindowsApi.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include "Core/Misc/PostWindowsApi.h"

class SD3D12Factory : public IRDIFactory
{
public:
	bool Init() noexcept;
	void Clear() noexcept;

	IDXGIFactory4* GetNativePtr() noexcept { return mDXGIFactoryNativePtr; }
	ID3D12Debug3* GetD3D12DebugNativePtr() noexcept { return mD3D12DebugNativePtr; }
	IDXGIDebug1* GetDXGIDebugNativePtr() noexcept { return mDXGIDebugNativePtr; }

public:
	const SRDIFutureSupport& GetFutureSupport() noexcept { return mFutureSupport; }
	std::string_view GetFactoryDesc() noexcept { return "D3D12 RDI"; }

	bool Create() noexcept override { return Init(); }
	void Release() noexcept override;

	IRDIDevice* GetDevice() noexcept override { return &mD3D12Device; }
	const std::vector<IRDIAdapter*>& GetAdapters() noexcept override { return mCachedAdapters; }

	SBlob CompileVertexShader(SConstBufferView _hlslShader, const SRDIShaderMacro* _shaderMacro, SRDIErrorInfo* _shaderCompileError) noexcept override;
	SBlob CompileHullShader(SConstBufferView _hlslShader, const SRDIShaderMacro* _shaderMacro, SRDIErrorInfo* _shaderCompileError) noexcept override;
	SBlob CompileDomainShader(SConstBufferView _hlslShader, const SRDIShaderMacro* _shaderMacro, SRDIErrorInfo* _shaderCompileError) noexcept override;
	SBlob CompileGeometryShader(SConstBufferView _hlslShader, const SRDIShaderMacro* _shaderMacro, SRDIErrorInfo* _shaderCompileError) noexcept override;
	SBlob CompilePixelShader(SConstBufferView _hlslShader, const SRDIShaderMacro* _shaderMacro, SRDIErrorInfo* _shaderCompileError) noexcept override;
	SBlob CompileComputeShader(SConstBufferView _hlslShader, const SRDIShaderMacro* _shaderMacro, SRDIErrorInfo* _shaderCompileError) noexcept override;
	
private:
	SBlob CompileShader(SConstBufferView _hlslShader, ED3DShaderTarget _shaderTarget, const SRDIShaderMacro* _shaderMacro, SRDIErrorInfo* _shaderCompileError) noexcept;

private:
	std::vector<IRDIAdapter*> mCachedAdapters;

private:
	IDXGIFactory4* mDXGIFactoryNativePtr = nullptr;
	ID3D12Debug3* mD3D12DebugNativePtr = nullptr;
	IDXGIDebug1* mDXGIDebugNativePtr = nullptr;

	SD3D12Device mD3D12Device;
	std::vector<SD3D12Adapter> mAdpaters;

	SRDIFutureSupport mFutureSupport = {};

	uint32_t mShaderCompileFlag = 0;
	D3DInclude mD3DInclude;
};
