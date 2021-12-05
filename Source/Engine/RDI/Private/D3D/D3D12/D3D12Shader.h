#pragma once
#include "Core/Container/Blob.h"
#include "RDI/Interface/RDIShader.h"

class SD3D12Device;

class SD3D12VertexShader : public IRDIVertexShader
{
public:
	void Init(SBufferView _buffer, SD3D12Device* _device) noexcept;
	void Init(SBlob&& _blob, SD3D12Device* _device) noexcept;

public:
	SBufferView GetCompiledShaderBlob() noexcept override { return SBufferView(mBlob); }
	void Release() noexcept override;

private:
	SD3D12Device* mDevice = nullptr;
	SBlob mBlob;
};

class SD3D12HullShader : public IRDIHullShader
{
public:
	void Init(SBufferView _buffer, SD3D12Device* _device) noexcept;
	void Init(SBlob&& _blob, SD3D12Device* _device) noexcept;

	SBufferView GetCompiledShaderBlob() noexcept override { return SBufferView(mBlob); }
	void Release() noexcept override;

private:
	SD3D12Device* mDevice = nullptr;
	SBlob mBlob;
};

class SD3D12DomainShader : public IRDIDomainShader
{
public:
	void Init(SBufferView _buffer, SD3D12Device* _device) noexcept;
	void Init(SBlob&& _blob, SD3D12Device* _device) noexcept;

	SBufferView GetCompiledShaderBlob() noexcept override { return SBufferView(mBlob); }
	void Release() noexcept override;

private:
	SD3D12Device* mDevice = nullptr;
	SBlob mBlob;
};

class SD3D12GeometryShader : public IRDIGeometryShader
{
public:
	void Init(SBufferView _buffer, SD3D12Device* _device) noexcept;
	void Init(SBlob&& _blob, SD3D12Device* _device) noexcept;

	SBufferView GetCompiledShaderBlob() noexcept override { return SBufferView(mBlob); }
	void Release() noexcept override;

private:
	SD3D12Device* mDevice = nullptr;
	SBlob mBlob;
};

class SD3D12PixelShader : public IRDIPixelShader
{
public:
	void Init(SBufferView _buffer, SD3D12Device* _device) noexcept;
	void Init(SBlob&& _blob, SD3D12Device* _device) noexcept;

	SBufferView GetCompiledShaderBlob() noexcept override { return SBufferView(mBlob); }
	void Release() noexcept override;

private:
	SD3D12Device* mDevice = nullptr;
	SBlob mBlob;
};

class SD3D12ComputeShader : public IRDIComputeShader
{
public:
	void Init(SBufferView _buffer, SD3D12Device* _device) noexcept;
	void Init(SBlob&& _blob, SD3D12Device* _device) noexcept;

	SBufferView GetCompiledShaderBlob() noexcept override { return SBufferView(mBlob); }
	void Release() noexcept override;

private:
	SD3D12Device* mDevice = nullptr;
	SBlob mBlob;
};
