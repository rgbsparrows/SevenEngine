#pragma once
#include "Core/Container/Blob.h"
#include "RDI/Interface/RDIShader.h"

class SD3D12ShaderBase
{
public:
	SD3D12ShaderBase(const void* _compiledShaderBuffer, size_t _bufferSize) noexcept
		:mBlob(_compiledShaderBuffer, _bufferSize)
	{
	}

	SD3D12ShaderBase(SBlob&& _compiledShaderBlob) noexcept
		:mBlob(std::forward<SBlob>(_compiledShaderBlob))
	{
	}

	SBlob mBlob;
};

class SD3D12VertexShader : public IRDIVertexShader, public SD3D12ShaderBase
{
public:
	SD3D12VertexShader(const void* _compiledShaderBuffer, size_t _bufferSize) noexcept
		:SD3D12ShaderBase(_compiledShaderBuffer, _bufferSize)
	{
	}

	SD3D12VertexShader(SBlob&& _compiledShaderBlob) noexcept
		:SD3D12ShaderBase(std::forward<SBlob>(_compiledShaderBlob))
	{
	}

	SBufferView GetCompiledShaderBlob() noexcept override
	{
		return SBufferView(mBlob);
	}
};

class SD3D12HullShader : public IRDIHullShader, public SD3D12ShaderBase
{
public:
	SD3D12HullShader(const void* _compiledShaderBuffer, size_t _bufferSize) noexcept
		:SD3D12ShaderBase(_compiledShaderBuffer, _bufferSize)
	{
	}

	SD3D12HullShader(SBlob&& _compiledShaderBlob) noexcept
		:SD3D12ShaderBase(std::forward<SBlob>(_compiledShaderBlob))
	{
	}

	SBufferView GetCompiledShaderBlob() noexcept override
	{
		return SBufferView(mBlob);
	}
};

class SD3D12DomainShader : public IRDIDomainShader, public SD3D12ShaderBase
{
public:
	SD3D12DomainShader(const void* _compiledShaderBuffer, size_t _bufferSize) noexcept
		:SD3D12ShaderBase(_compiledShaderBuffer, _bufferSize)
	{
	}

	SD3D12DomainShader(SBlob&& _compiledShaderBlob) noexcept
		:SD3D12ShaderBase(std::forward<SBlob>(_compiledShaderBlob))
	{
	}

	SBufferView GetCompiledShaderBlob() noexcept override
	{
		return SBufferView(mBlob);
	}
};

class SD3D12GeometryShader : public IRDIGeometryShader, public SD3D12ShaderBase
{
public:
	SD3D12GeometryShader(const void* _compiledShaderBuffer, size_t _bufferSize) noexcept
		:SD3D12ShaderBase(_compiledShaderBuffer, _bufferSize)
	{
	}

	SD3D12GeometryShader(SBlob&& _compiledShaderBlob) noexcept
		:SD3D12ShaderBase(std::forward<SBlob>(_compiledShaderBlob))
	{
	}

	SBufferView GetCompiledShaderBlob() noexcept override
	{
		return SBufferView(mBlob);
	}
};

class SD3D12PixelShader : public IRDIPixelShader, public SD3D12ShaderBase
{
public:
	SD3D12PixelShader(const void* _compiledShaderBuffer, size_t _bufferSize) noexcept
		:SD3D12ShaderBase(_compiledShaderBuffer, _bufferSize)
	{
	}

	SD3D12PixelShader(SBlob&& _compiledShaderBlob) noexcept
		:SD3D12ShaderBase(std::forward<SBlob>(_compiledShaderBlob))
	{
	}

	SBufferView GetCompiledShaderBlob() noexcept override
	{
		return SBufferView(mBlob);
	}
};

class SD3D12ComputeShader : public IRDIComputeShader, public SD3D12ShaderBase
{
public:
	SD3D12ComputeShader(const void* _compiledShaderBuffer, size_t _bufferSize) noexcept
		:SD3D12ShaderBase(_compiledShaderBuffer, _bufferSize)
	{
	}

	SD3D12ComputeShader(SBlob&& _compiledShaderBlob) noexcept
		:SD3D12ShaderBase(std::forward<SBlob>(_compiledShaderBlob))
	{
	}

	SBufferView GetCompiledShaderBlob() noexcept override
	{
		return SBufferView(mBlob);
	}
};
