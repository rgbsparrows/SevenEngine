#pragma once

class SBufferView;

__interface IRDIShader
{
	SBufferView GetCompiledShaderBlob() noexcept;
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
