#include "Core/Localize.h"
#include "D3D12InputLayout.h"
#include "D3D/Warper/D3DEnumConvertor.h"

void SD3D12InputLayout::Init(const SRDIVertexInputLayoutDesc* _desc) noexcept
{
	for (auto& _ele : _desc->mInputElements)
	{
		mSemanticNames.push_back(Locale::ConvertWstringToString(Locale::ECodePage::ACP, _ele.mSemanticName));

		D3D12_INPUT_ELEMENT_DESC elementDesc;
		elementDesc.SemanticName = nullptr;
		elementDesc.SemanticIndex = 0;
		elementDesc.Format = ConvertPixelFormatToD3D(_ele.mFormat);
		elementDesc.InputSlot = 0;
		elementDesc.AlignedByteOffset = _ele.mAlignedByteOffset;
		elementDesc.InputSlotClass = D3D12_INPUT_CLASSIFICATION::D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;

		mInputElements.push_back(elementDesc);
	}

	for (size_t i = 0; i != _desc->mInputElements.size(); ++i)
	{
		mInputElements[i].SemanticName = mSemanticNames[i].c_str();
	}

	mDesc.pInputElementDescs = mInputElements.data();
	mDesc.NumElements = static_cast<uint32_t>(mInputElements.size());
}
