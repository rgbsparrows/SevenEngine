#include "RDI/RDIFunctionHelper.h"
#include "RDI/Interface/RDIInputLayout.h"
#include "D3D/D3D12/Warper/D3D12ImplWarperHelper.h"

class SD3D12InputLayout : public IRDIInputLayout
{
public:
	void Init(const SRDIVertexInputLayoutDesc* _desc) noexcept;
	const D3D12_INPUT_LAYOUT_DESC& GetD3D12InputLayout() noexcept { return mDesc; }

private:
	D3D12_INPUT_LAYOUT_DESC mDesc = {};

	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputElements;
	std::vector<std::string> mSemanticNames;
};
