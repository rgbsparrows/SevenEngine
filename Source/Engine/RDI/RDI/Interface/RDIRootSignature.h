#pragma once

class SBlob;

__interface IRDIRootSignature
{
	SBufferView GetSerializedRootSignature() noexcept;
};
