#pragma once

#include "RDI/RDIFunctionHelper.h"
#include "RDI/Interface/RDIInputLayout.h"

#include "Core/Misc/PreWindowsApi.h"
#include <d3d12.h>
#include "Core/Misc/PostWindowsApi.h"

class SD3D12Device;

class SD3D12InputLayout : public IRDIInputLayout
{
public:
	void Init(const SRDIVertexInputLayoutDesc* _desc, SD3D12Device* _device) noexcept;
	const D3D12_INPUT_LAYOUT_DESC& GetD3D12InputLayout() noexcept { return mDesc; }

public:
	void Release() noexcept override;

private:
	SD3D12Device* mDevice = nullptr;
	D3D12_INPUT_LAYOUT_DESC mDesc = {};

	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputElements;
	std::vector<std::string> mSemanticNames;
};
