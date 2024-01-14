#pragma once

#include "Core/Container/Blob.h"
#include "Core/Util/UtilMacros.h"
#include "Core/Util/TemplateUtil.h"

#include <vector>
#include <stdint.h>
#include <filesystem>
#include <string_view>

struct SRDIFutureSupport;

__interface IRDIDevice;
__interface IRDIAdapter;
__interface IRDIResource;
__interface IRDICommandList;
__interface IRDICommandQueue;

struct SRDIErrorInfo;
struct SRDIShaderMacro;

__interface IRDIFactory
{
	const SRDIFutureSupport& GetFutureSupport() noexcept;
	std::string_view GetFactoryDesc() noexcept;

	bool Create() noexcept;
	void Release() noexcept;

	IRDIDevice* GetDevice() noexcept;
	const std::vector<IRDIAdapter*>& GetAdapters() noexcept;

	SBlob CompileVertexShader(SConstBufferView _hlslShader, const SRDIShaderMacro* _shaderMacro = nullptr, SRDIErrorInfo* _shaderCompileError = nullptr) noexcept;
	SBlob CompileHullShader(SConstBufferView _hlslShader, const SRDIShaderMacro* _shaderMacro = nullptr, SRDIErrorInfo* _shaderCompileError = nullptr) noexcept;
	SBlob CompileDomainShader(SConstBufferView _hlslShader, const SRDIShaderMacro* _shaderMacro = nullptr, SRDIErrorInfo* _shaderCompileError = nullptr) noexcept;
	SBlob CompileGeometryShader(SConstBufferView _hlslShader, const SRDIShaderMacro* _shaderMacro = nullptr, SRDIErrorInfo* _shaderCompileError = nullptr) noexcept;
	SBlob CompilePixelShader(SConstBufferView _hlslShader, const SRDIShaderMacro* _shaderMacro = nullptr, SRDIErrorInfo* _shaderCompileError = nullptr) noexcept;
	SBlob CompileComputeShader(SConstBufferView _hlslShader, const SRDIShaderMacro* _shaderMacro = nullptr, SRDIErrorInfo* _shaderCompileError = nullptr) noexcept;
};
