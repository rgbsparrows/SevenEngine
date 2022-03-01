#include "D3D12Shader.h"
#include "D3D12Device.h"

void SD3D12VertexShader::Init(SBufferView _buffer, SD3D12Device* _device) noexcept
{
	mDevice = _device;
	mBlob.ResizeBlob(_buffer.GetBuffer(), _buffer.GetBufferSize());
}

void SD3D12VertexShader::Init(SBlob&& _blob, SD3D12Device* _device) noexcept
{
	mDevice = _device;
	mBlob = std::move(_blob);
}

void SD3D12VertexShader::Release() noexcept
{
	mDevice->ReleaseVertexShader(this);
}

void SD3D12HullShader::Init(SBufferView _buffer, SD3D12Device* _device) noexcept
{
	mDevice = _device;
	mBlob.ResizeBlob(_buffer.GetBuffer(), _buffer.GetBufferSize());
}

void SD3D12HullShader::Init(SBlob&& _blob, SD3D12Device* _device) noexcept
{
	mDevice = _device;
	mBlob = std::move(_blob);
}

void SD3D12HullShader::Release() noexcept
{
	mDevice->ReleaseHullShader(this);
}

void SD3D12DomainShader::Init(SBufferView _buffer, SD3D12Device* _device) noexcept
{
	mDevice = _device;
	mBlob.ResizeBlob(_buffer.GetBuffer(), _buffer.GetBufferSize());
}

void SD3D12DomainShader::Init(SBlob&& _blob, SD3D12Device* _device) noexcept
{
	mDevice = _device;
	mBlob = std::move(_blob);
}

void SD3D12DomainShader::Release() noexcept
{
	mDevice->ReleaseDomainShader(this);
}

void SD3D12GeometryShader::Init(SBufferView _buffer, SD3D12Device* _device) noexcept
{
	mDevice = _device;
	mBlob.ResizeBlob(_buffer.GetBuffer(), _buffer.GetBufferSize());
}

void SD3D12GeometryShader::Init(SBlob&& _blob, SD3D12Device* _device) noexcept
{
	mDevice = _device;
	mBlob = std::move(_blob);
}

void SD3D12GeometryShader::Release() noexcept
{
	mDevice->ReleaseGeometryShader(this);
}

void SD3D12PixelShader::Init(SBufferView _buffer, SD3D12Device* _device) noexcept
{
	mDevice = _device;
	mBlob.ResizeBlob(_buffer.GetBuffer(), _buffer.GetBufferSize());
}

void SD3D12PixelShader::Init(SBlob&& _blob, SD3D12Device* _device) noexcept
{
	mDevice = _device;
	mBlob = std::move(_blob);
}

void SD3D12PixelShader::Release() noexcept
{
	mDevice->ReleasePixelShader(this);
}

void SD3D12ComputeShader::Init(SBufferView _buffer, SD3D12Device* _device) noexcept
{
	mDevice = _device;
	mBlob.ResizeBlob(_buffer.GetBuffer(), _buffer.GetBufferSize());
}

void SD3D12ComputeShader::Init(SBlob&& _blob, SD3D12Device* _device) noexcept
{
	mDevice = _device;
	mBlob = std::move(_blob);
}

void SD3D12ComputeShader::Release() noexcept
{
	mDevice->ReleaseComputeShader(this);
}
