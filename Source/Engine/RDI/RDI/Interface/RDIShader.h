#pragma once
#include "Core/Container/Blob.h"

__interface IRDIShader
{
	SConstBufferView GetCompiledShaderBlob() noexcept;
	void Release() noexcept;
};

__interface IRDIVertexShader : IRDIShader
{
};

__interface IRDIHullShader : IRDIShader
{
};

__interface IRDIDomainShader : IRDIShader
{
};

__interface IRDIGeometryShader : IRDIShader
{
};

__interface IRDIPixelShader : IRDIShader
{
};

__interface IRDIComputeShader : IRDIShader
{
};
