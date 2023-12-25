#pragma once

class SBlob;

__interface IRDIRootSignature
{
	SConstBufferView GetSerializedRootSignature() noexcept;
	void Release() noexcept;
};
