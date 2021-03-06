#include "D3D/D3DUtil.h"
#include "D3D/D3D12/D3D12Device.h"
#include "D3D/D3D12/D3D12Resource.h"
#include "D3D/D3D12/D3D12CommandList.h"
#include "D3D/D3D12/D3D12RootSignature.h"
#include "D3D/D3D12/D3D12PipelineState.h"
#include "D3D/D3D12/D3D12DescriptorHeap.h"
#include "D3D/D3D12/Helper/D3D12EnumConvertor.h"

void SD3D12CommandList::Init(ID3D12CommandAllocator* _commandAllocatorNativePtr, ID3D12GraphicsCommandList* _commandListNativePtr, SD3D12Device* _device) noexcept
{
	{
		mCommandAllocatorNativePtr = _commandAllocatorNativePtr;
		mCommandListNativePtr = _commandListNativePtr;
		mDevice = _device;
	}
}

void SD3D12CommandList::ResetCommandAllocator() noexcept
{
	VERIFY_D3D_RETURN(GetCommandAllocatorNativePtr()->Reset());
}

void SD3D12CommandList::ResetCommandList() noexcept
{
	VERIFY_D3D_RETURN(GetCommandListNativePtr()->Reset(GetCommandAllocatorNativePtr(), nullptr));

	ID3D12DescriptorHeap* descriptorHeaps[] =
	{
		mDevice->GetShaderVisibleDescriptorHeap()->GetSrvUavDescriptorHeapNativePtr(),
		mDevice->GetShaderVisibleDescriptorHeap()->GetSamplerViewDescriptorHeapNativePtr(),
	};
	GetCommandListNativePtr()->SetDescriptorHeaps(static_cast<UINT>(ArraySize(descriptorHeaps)), descriptorHeaps);
}

void SD3D12CommandList::Close() noexcept
{
	VERIFY_D3D_RETURN(GetCommandListNativePtr()->Close());
}

void SD3D12CommandList::DrawIndexedInstanced(uint32_t _indexCount, uint32_t _instanceCount, uint32_t _startIndexLocation, uint32_t _baseVertexLocation) noexcept
{
	GetCommandListNativePtr()->DrawIndexedInstanced(_indexCount, _instanceCount, _startIndexLocation, _baseVertexLocation, 0);
}

void SD3D12CommandList::Dispatch(uint32_t _threadGroupX, uint32_t _threadGroupY, uint32_t _threadGroupZ) noexcept
{
	GetCommandListNativePtr()->Dispatch(_threadGroupX, _threadGroupY, _threadGroupZ);
}

void SD3D12CommandList::CopyBufferRegion(IRDIBuffer* _destBuffer, uint64_t _destOffset, IRDIBuffer* _srcBuffer, uint64_t _srcOffset, uint64_t _numByte) noexcept
{
	SD3D12Buffer* destBuffer = static_cast<SD3D12Buffer*>(_destBuffer);
	SD3D12Buffer* srcBuffer = static_cast<SD3D12Buffer*>(_srcBuffer);

	GetCommandListNativePtr()->CopyBufferRegion(destBuffer->GetNativePtr(), _destOffset, srcBuffer->GetNativePtr(), _srcOffset, _numByte);
}

void SD3D12CommandList::CopyBuffer(IRDIBuffer* _destBuffer, IRDIBuffer* _srcBuffer) noexcept
{
	SD3D12Buffer* destBuffer = static_cast<SD3D12Buffer*>(_destBuffer);
	SD3D12Buffer* srcBuffer = static_cast<SD3D12Buffer*>(_srcBuffer);

	GetCommandListNativePtr()->CopyResource(destBuffer->GetNativePtr(), srcBuffer->GetNativePtr());
}

void SD3D12CommandList::CopyTexture1D(IRDITexture1D* _destTexture, IRDITexture1D* _srcTexture) noexcept
{
	SD3D12Texture1D* destTexture = static_cast<SD3D12Texture1D*>(_destTexture);
	SD3D12Texture1D* srcTexture = static_cast<SD3D12Texture1D*>(_srcTexture);

	GetCommandListNativePtr()->CopyResource(destTexture->GetNativePtr(), srcTexture->GetNativePtr());
}

void SD3D12CommandList::CopyTexture1DArray(IRDITexture1DArray* _destTexture, IRDITexture1DArray* _srcTexture) noexcept
{
	SD3D12Texture1DArray* destTexture = static_cast<SD3D12Texture1DArray*>(_destTexture);
	SD3D12Texture1DArray* srcTexture = static_cast<SD3D12Texture1DArray*>(_srcTexture);

	GetCommandListNativePtr()->CopyResource(destTexture->GetNativePtr(), srcTexture->GetNativePtr());
}

void SD3D12CommandList::CopyTexture2D(IRDITexture2D* _destTexture, IRDITexture2D* _srcTexture) noexcept
{
	SD3D12Texture2D* destTexture = static_cast<SD3D12Texture2D*>(_destTexture);
	SD3D12Texture2D* srcTexture = static_cast<SD3D12Texture2D*>(_srcTexture);

	GetCommandListNativePtr()->CopyResource(destTexture->GetNativePtr(), srcTexture->GetNativePtr());
}

void SD3D12CommandList::CopyTexture2DArray(IRDITexture2DArray* _destTexture, IRDITexture2DArray* _srcTexture) noexcept
{
	SD3D12Texture2DArray* destTexture = static_cast<SD3D12Texture2DArray*>(_destTexture);
	SD3D12Texture2DArray* srcTexture = static_cast<SD3D12Texture2DArray*>(_srcTexture);

	GetCommandListNativePtr()->CopyResource(destTexture->GetNativePtr(), srcTexture->GetNativePtr());
}

void SD3D12CommandList::CopyTexture3D(IRDITexture3D* _destTexture, IRDITexture3D* _srcTexture) noexcept
{
	SD3D12Texture3D* destTexture = static_cast<SD3D12Texture3D*>(_destTexture);
	SD3D12Texture3D* srcTexture = static_cast<SD3D12Texture3D*>(_srcTexture);

	GetCommandListNativePtr()->CopyResource(destTexture->GetNativePtr(), srcTexture->GetNativePtr());
}

void SD3D12CommandList::CopyTextureCube(IRDITextureCube* _destTexture, IRDITextureCube* _srcTexture) noexcept
{
	SD3D12TextureCube* destTexture = static_cast<SD3D12TextureCube*>(_destTexture);
	SD3D12TextureCube* srcTexture = static_cast<SD3D12TextureCube*>(_srcTexture);

	GetCommandListNativePtr()->CopyResource(destTexture->GetNativePtr(), srcTexture->GetNativePtr());
}

void SD3D12CommandList::CopyTextureCubeArray(IRDITextureCubeArray* _destTexture, IRDITextureCubeArray* _srcTexture) noexcept
{
	SD3D12TextureCubeArray* destTexture = static_cast<SD3D12TextureCubeArray*>(_destTexture);
	SD3D12TextureCubeArray* srcTexture = static_cast<SD3D12TextureCubeArray*>(_srcTexture);

	GetCommandListNativePtr()->CopyResource(destTexture->GetNativePtr(), srcTexture->GetNativePtr());
}

void SD3D12CommandList::UnorderAccessResourceBarrier(IRDIBuffer* _resource) noexcept
{
	SD3D12Buffer* resource = static_cast<SD3D12Buffer*>(_resource);

	UnorderAccessResourceBarrier(resource->GetNativePtr());
}

void SD3D12CommandList::UnorderAccessResourceBarrier(IRDITexture1D* _resource) noexcept
{
	SD3D12Texture1D* resource = static_cast<SD3D12Texture1D*>(_resource);

	UnorderAccessResourceBarrier(resource->GetNativePtr());
}

void SD3D12CommandList::UnorderAccessResourceBarrier(IRDITexture1DArray* _resource) noexcept
{
	SD3D12Texture1DArray* resource = static_cast<SD3D12Texture1DArray*>(_resource);

	UnorderAccessResourceBarrier(resource->GetNativePtr());
}

void SD3D12CommandList::UnorderAccessResourceBarrier(IRDITexture2D* _resource) noexcept
{
	SD3D12Texture2D* resource = static_cast<SD3D12Texture2D*>(_resource);

	UnorderAccessResourceBarrier(resource->GetNativePtr());
}

void SD3D12CommandList::UnorderAccessResourceBarrier(IRDITexture2DArray* _resource) noexcept
{
	SD3D12Texture2DArray* resource = static_cast<SD3D12Texture2DArray*>(_resource);

	UnorderAccessResourceBarrier(resource->GetNativePtr());
}

void SD3D12CommandList::UnorderAccessResourceBarrier(IRDITexture3D* _resource) noexcept
{
	SD3D12Texture3D* resource = static_cast<SD3D12Texture3D*>(_resource);

	UnorderAccessResourceBarrier(resource->GetNativePtr());
}

void SD3D12CommandList::UnorderAccessResourceBarrier(IRDITextureCube* _resource) noexcept
{
	SD3D12TextureCube* resource = static_cast<SD3D12TextureCube*>(_resource);

	UnorderAccessResourceBarrier(resource->GetNativePtr());
}

void SD3D12CommandList::UnorderAccessResourceBarrier(IRDITextureCubeArray* _resource) noexcept
{
	SD3D12TextureCubeArray* resource = static_cast<SD3D12TextureCubeArray*>(_resource);

	UnorderAccessResourceBarrier(resource->GetNativePtr());
}

void SD3D12CommandList::TranstionResourceState(IRDIBuffer* _resource, ERDIResourceState _before, ERDIResourceState _after) noexcept
{
	SD3D12Buffer* resource = static_cast<SD3D12Buffer*>(_resource);

	TranstionResourceState(resource->GetNativePtr(), _before, _after);
}

void SD3D12CommandList::TranstionResourceState(IRDITexture1D* _resource, ERDIResourceState _before, ERDIResourceState _after) noexcept
{
	SD3D12Texture1D* resource = static_cast<SD3D12Texture1D*>(_resource);

	TranstionResourceState(resource->GetNativePtr(), _before, _after);
}

void SD3D12CommandList::TranstionResourceState(IRDITexture1DArray* _resource, ERDIResourceState _before, ERDIResourceState _after) noexcept
{
	SD3D12Texture1DArray* resource = static_cast<SD3D12Texture1DArray*>(_resource);

	TranstionResourceState(resource->GetNativePtr(), _before, _after);
}

void SD3D12CommandList::TranstionResourceState(IRDITexture2D* _resource, ERDIResourceState _before, ERDIResourceState _after) noexcept
{
	SD3D12Texture2D* resource = static_cast<SD3D12Texture2D*>(_resource);

	TranstionResourceState(resource->GetNativePtr(), _before, _after);
}

void SD3D12CommandList::TranstionResourceState(IRDITexture2DArray* _resource, ERDIResourceState _before, ERDIResourceState _after) noexcept
{
	SD3D12Texture2DArray* resource = static_cast<SD3D12Texture2DArray*>(_resource);

	TranstionResourceState(resource->GetNativePtr(), _before, _after);
}

void SD3D12CommandList::TranstionResourceState(IRDITexture3D* _resource, ERDIResourceState _before, ERDIResourceState _after) noexcept
{
	SD3D12Texture3D* resource = static_cast<SD3D12Texture3D*>(_resource);

	TranstionResourceState(resource->GetNativePtr(), _before, _after);
}

void SD3D12CommandList::TranstionResourceState(IRDITextureCube* _resource, ERDIResourceState _before, ERDIResourceState _after) noexcept
{
	SD3D12TextureCube* resource = static_cast<SD3D12TextureCube*>(_resource);

	TranstionResourceState(resource->GetNativePtr(), _before, _after);
}

void SD3D12CommandList::TranstionResourceState(IRDITextureCubeArray* _resource, ERDIResourceState _before, ERDIResourceState _after) noexcept
{
	SD3D12TextureCubeArray* resource = static_cast<SD3D12TextureCubeArray*>(_resource);

	TranstionResourceState(resource->GetNativePtr(), _before, _after);
}

void SD3D12CommandList::IASetPrimitiveTopology(ERDIPrimitiveTopology _primitiveTopology) noexcept
{
	GetCommandListNativePtr()->IASetPrimitiveTopology(ConvertPrimitiveTopologyToD3D12(_primitiveTopology));
}

void SD3D12CommandList::IASetIndexBuffer(IRDIIndexBufferView* _ibv) noexcept
{
	SD3D12IndexBufferView* ibv = static_cast<SD3D12IndexBufferView*>(_ibv);

	D3D12_INDEX_BUFFER_VIEW viewDesc;
	viewDesc.BufferLocation = ibv->mGpuVirtualAddress;
	viewDesc.SizeInBytes = ibv->mSizeInByte;
	viewDesc.Format = ibv->mFormat;

	GetCommandListNativePtr()->IASetIndexBuffer(&viewDesc);
}

void SD3D12CommandList::IASetVertexBuffer(uint32_t _slotIndex, uint32_t _bufferCount, IRDIVertexBufferView* const* _vbv) noexcept
{
	CHECK(_slotIndex + _bufferCount <= D3D12_VERTEX_BUFFER_SLOT_COUNT);

	for (size_t i = 0; i != _bufferCount; ++i)
	{
		SD3D12VertexBufferView* vbv = static_cast<SD3D12VertexBufferView*>(_vbv[i]);

		mViewDescCache[i].BufferLocation = vbv->mGpuVirtualAddress;
		mViewDescCache[i].SizeInBytes = vbv->mSizeInByte;
		mViewDescCache[i].StrideInBytes = vbv->mStrideInBytes;
	}

	GetCommandListNativePtr()->IASetVertexBuffers(_slotIndex, _bufferCount, mViewDescCache);
}

void SD3D12CommandList::RSSetViewports(uint32_t _numViewports, const Math::SFloatBox* _viewports) noexcept
{
	for (size_t i = 0; i != _numViewports; ++i)
	{
		mViewportsCache[i].TopLeftX = _viewports[i].mLeftOnFront[0];
		mViewportsCache[i].TopLeftY = _viewports[i].mLeftOnFront[1];
		mViewportsCache[i].MinDepth = _viewports[i].mLeftOnFront[2];
		mViewportsCache[i].Width = _viewports[i].mRightDwonBack[0] - _viewports[i].mLeftOnFront[0];
		mViewportsCache[i].Height = _viewports[i].mRightDwonBack[1] - _viewports[i].mLeftOnFront[1];
		mViewportsCache[i].MaxDepth = _viewports[i].mRightDwonBack[2];
	}

	GetCommandListNativePtr()->RSSetViewports(_numViewports, mViewportsCache);
}

void SD3D12CommandList::RSSetScissorRects(uint32_t _numRects, const Math::SIntRect* _rects) noexcept
{
	for (size_t i = 0; i != _numRects; ++i)
	{
		mScissorRectCache[i].left = _rects[i].mLeftOn[0];
		mScissorRectCache[i].top = _rects[i].mLeftOn[1];
		mScissorRectCache[i].right = _rects[i].mRightDwon[0];
		mScissorRectCache[i].bottom = _rects[i].mRightDwon[1];
	}

	GetCommandListNativePtr()->RSSetScissorRects(_numRects, mScissorRectCache);
}

void SD3D12CommandList::OMSetRenderTargets(uint32_t _renderTargetCount, IRDIRenderTargetView* const* _rtvs, IRDIDepthStencilView* _dsv) noexcept
{
	for (size_t i = 0; i != _renderTargetCount; ++i)
	{
		SD3D12RenderTargetView* rtv = static_cast<SD3D12RenderTargetView*>(_rtvs[i]);

		mRtvDescriptorHandlesCache[i] = rtv->mCpuDescriptorHandle;
	}

	SD3D12DepthStencilView* dsv = static_cast<SD3D12DepthStencilView*>(_dsv);
	D3D12_CPU_DESCRIPTOR_HANDLE dsvDescriptorHandle = dsv->mCpuDescriptorHandle;

	GetCommandListNativePtr()->OMSetRenderTargets(_renderTargetCount, mRtvDescriptorHandlesCache, false, &dsvDescriptorHandle);
}

void SD3D12CommandList::OMSetBlendFactor(Math::SFColor _blendFactor) noexcept
{
	GetCommandListNativePtr()->OMSetBlendFactor(_blendFactor.Color);
}

void SD3D12CommandList::OMSetStencilRef(uint32_t _stencilRef) noexcept
{
	GetCommandListNativePtr()->OMSetStencilRef(_stencilRef);
}

void SD3D12CommandList::SetGraphicsPipelineState(IRDIGraphicsPipelineState* _pipelineState) noexcept
{
	SD3D12GraphicsPipelineState* pipelineState = static_cast<SD3D12GraphicsPipelineState*>(_pipelineState);

	GetCommandListNativePtr()->SetPipelineState(pipelineState->GetNativePtr());
}

void SD3D12CommandList::SetComputePipelineState(IRDIComputePipelineState* _pipelineState) noexcept
{
	SD3D12ComputePipelineState* pipelineState = static_cast<SD3D12ComputePipelineState*>(_pipelineState);

	GetCommandListNativePtr()->SetPipelineState(pipelineState->GetNativePtr());
}

void SD3D12CommandList::SetGraphicsRootSignature(IRDIRootSignature* _rootSignature) noexcept
{
	SD3D12RootSignature* rootSignature = static_cast<SD3D12RootSignature*>(_rootSignature);

	GetCommandListNativePtr()->SetGraphicsRootSignature(rootSignature->GetNativePtr());
}

void SD3D12CommandList::SetComputeRootSignature(IRDIRootSignature* _rootSignature) noexcept
{
	SD3D12RootSignature* rootSignature = static_cast<SD3D12RootSignature*>(_rootSignature);

	GetCommandListNativePtr()->SetComputeRootSignature(rootSignature->GetNativePtr());
}

void SD3D12CommandList::SetGraphicsRootConstantBuffer(uint32_t _rootParameterIndex, IRDIBuffer* _buffer, uint64_t _offset) noexcept
{
	SD3D12Buffer* buffer = static_cast<SD3D12Buffer*>(_buffer);
	GetCommandListNativePtr()->SetGraphicsRootConstantBufferView(_rootParameterIndex, buffer->GetGpuVirtualAddress() + _offset);
}

void SD3D12CommandList::SetComputeRootConstantBuffer(uint32_t _rootParameterIndex, IRDIBuffer* _buffer, uint64_t _offset) noexcept
{
	SD3D12Buffer* buffer = static_cast<SD3D12Buffer*>(_buffer);

	GetCommandListNativePtr()->SetComputeRootConstantBufferView(_rootParameterIndex, buffer->GetGpuVirtualAddress() + _offset);
}

void SD3D12CommandList::SetGraphicsRootDescriptorTable(uint32_t _rootParameterIndex, IRDIDescriptorHeapRange* _descriptorRange) noexcept
{
	SD3D12DescriptorHeapRange* descriptorHeapRange = static_cast<SD3D12DescriptorHeapRange*>(_descriptorRange);

	GetCommandListNativePtr()->SetGraphicsRootDescriptorTable(_rootParameterIndex, descriptorHeapRange->GetGpuDescriptorHandle());
}

void SD3D12CommandList::SetComputeRootDescriptorTable(uint32_t _rootParameterIndex, IRDIDescriptorHeapRange* _descriptorRange) noexcept
{
	SD3D12DescriptorHeapRange* descriptorHeapRange = static_cast<SD3D12DescriptorHeapRange*>(_descriptorRange);

	GetCommandListNativePtr()->SetComputeRootDescriptorTable(_rootParameterIndex, descriptorHeapRange->GetGpuDescriptorHandle());
}

void SD3D12CommandList::SetGraphicsRootSamplerDescriptorTable(uint32_t _rootParameterIndex, IRDISamplerHeapRange* _samplerRange) noexcept
{
	SD3D12SamplerHeapRange* samplerRange = static_cast<SD3D12SamplerHeapRange*>(_samplerRange);

	GetCommandListNativePtr()->SetGraphicsRootDescriptorTable(_rootParameterIndex, samplerRange->GetGpuDescriptorHandle());
}

void SD3D12CommandList::SetComputeRootSamplerDescriptorTable(uint32_t _rootParameterIndex, IRDISamplerHeapRange* _samplerRange) noexcept
{
	SD3D12SamplerHeapRange* samplerRange = static_cast<SD3D12SamplerHeapRange*>(_samplerRange);

	GetCommandListNativePtr()->SetComputeRootDescriptorTable(_rootParameterIndex, samplerRange->GetGpuDescriptorHandle());
}

void SD3D12CommandList::ClearDepthStencilView(IRDIDepthStencilView* _dsv, ERDIClearFlag _clearFlag, float _depth, uint8_t _stencil) noexcept
{
	SD3D12DepthStencilView* dsv = static_cast<SD3D12DepthStencilView*>(_dsv);
	D3D12_CLEAR_FLAGS clearFlags = ConvertClearFlagToD3D12(_clearFlag);

	GetCommandListNativePtr()->ClearDepthStencilView(dsv->mCpuDescriptorHandle, clearFlags, _depth, _stencil, 0, nullptr);
}

void SD3D12CommandList::ClearRenderTargetView(IRDIRenderTargetView* _rtv, Math::SFColor _color) noexcept
{
	SD3D12RenderTargetView* rtv = static_cast<SD3D12RenderTargetView*>(_rtv);

	GetCommandListNativePtr()->ClearRenderTargetView(rtv->mCpuDescriptorHandle, _color.Color, 0, nullptr);
}

void SD3D12CommandList::ClearUnorderAccessViewUINT(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDIBuffer* _resource, Math::SUColor _value) noexcept
{
	SD3D12DescriptorHeapRange* descriptorHeapRange = static_cast<SD3D12DescriptorHeapRange*>(_shaderVisibleUav);
	SD3D12UnorderedAccessView* uav = static_cast<SD3D12UnorderedAccessView*>(_uav);
	SD3D12Buffer* resource = static_cast<SD3D12Buffer*>(_resource);

	ClearUnorderAccessViewUINT(descriptorHeapRange->GetGpuDescriptorHandle(), uav->mCpuDescriptorHandle, resource->GetNativePtr(), _value);
}

void SD3D12CommandList::ClearUnorderAccessViewUINT(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDITexture1D* _resource, Math::SUColor _value) noexcept
{
	SD3D12DescriptorHeapRange* descriptorHeapRange = static_cast<SD3D12DescriptorHeapRange*>(_shaderVisibleUav);
	SD3D12UnorderedAccessView* uav = static_cast<SD3D12UnorderedAccessView*>(_uav);
	SD3D12Texture1D* resource = static_cast<SD3D12Texture1D*>(_resource);

	ClearUnorderAccessViewUINT(descriptorHeapRange->GetGpuDescriptorHandle(), uav->mCpuDescriptorHandle, resource->GetNativePtr(), _value);
}

void SD3D12CommandList::ClearUnorderAccessViewUINT(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDITexture1DArray* _resource, Math::SUColor _value) noexcept
{
	SD3D12DescriptorHeapRange* descriptorHeapRange = static_cast<SD3D12DescriptorHeapRange*>(_shaderVisibleUav);
	SD3D12UnorderedAccessView* uav = static_cast<SD3D12UnorderedAccessView*>(_uav);
	SD3D12Texture1DArray* resource = static_cast<SD3D12Texture1DArray*>(_resource);

	ClearUnorderAccessViewUINT(descriptorHeapRange->GetGpuDescriptorHandle(), uav->mCpuDescriptorHandle, resource->GetNativePtr(), _value);
}

void SD3D12CommandList::ClearUnorderAccessViewUINT(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDITexture2D* _resource, Math::SUColor _value) noexcept
{
	SD3D12DescriptorHeapRange* descriptorHeapRange = static_cast<SD3D12DescriptorHeapRange*>(_shaderVisibleUav);
	SD3D12UnorderedAccessView* uav = static_cast<SD3D12UnorderedAccessView*>(_uav);
	SD3D12Texture2D* resource = static_cast<SD3D12Texture2D*>(_resource);

	ClearUnorderAccessViewUINT(descriptorHeapRange->GetGpuDescriptorHandle(), uav->mCpuDescriptorHandle, resource->GetNativePtr(), _value);
}

void SD3D12CommandList::ClearUnorderAccessViewUINT(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDITexture2DArray* _resource, Math::SUColor _value) noexcept
{
	SD3D12DescriptorHeapRange* descriptorHeapRange = static_cast<SD3D12DescriptorHeapRange*>(_shaderVisibleUav);
	SD3D12UnorderedAccessView* uav = static_cast<SD3D12UnorderedAccessView*>(_uav);
	SD3D12Texture2DArray* resource = static_cast<SD3D12Texture2DArray*>(_resource);

	ClearUnorderAccessViewUINT(descriptorHeapRange->GetGpuDescriptorHandle(), uav->mCpuDescriptorHandle, resource->GetNativePtr(), _value);
}

void SD3D12CommandList::ClearUnorderAccessViewUINT(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDITexture3D* _resource, Math::SUColor _value) noexcept
{
	SD3D12DescriptorHeapRange* descriptorHeapRange = static_cast<SD3D12DescriptorHeapRange*>(_shaderVisibleUav);
	SD3D12UnorderedAccessView* uav = static_cast<SD3D12UnorderedAccessView*>(_uav);
	SD3D12Texture3D* resource = static_cast<SD3D12Texture3D*>(_resource);

	ClearUnorderAccessViewUINT(descriptorHeapRange->GetGpuDescriptorHandle(), uav->mCpuDescriptorHandle, resource->GetNativePtr(), _value);
}

void SD3D12CommandList::ClearUnorderAccessViewUINT(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDITextureCube* _resource, Math::SUColor _value) noexcept
{
	SD3D12DescriptorHeapRange* descriptorHeapRange = static_cast<SD3D12DescriptorHeapRange*>(_shaderVisibleUav);
	SD3D12UnorderedAccessView* uav = static_cast<SD3D12UnorderedAccessView*>(_uav);
	SD3D12TextureCube* resource = static_cast<SD3D12TextureCube*>(_resource);

	ClearUnorderAccessViewUINT(descriptorHeapRange->GetGpuDescriptorHandle(), uav->mCpuDescriptorHandle, resource->GetNativePtr(), _value);
}

void SD3D12CommandList::ClearUnorderAccessViewUINT(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDITextureCubeArray* _resource, Math::SUColor _value) noexcept
{
	SD3D12DescriptorHeapRange* descriptorHeapRange = static_cast<SD3D12DescriptorHeapRange*>(_shaderVisibleUav);
	SD3D12UnorderedAccessView* uav = static_cast<SD3D12UnorderedAccessView*>(_uav);
	SD3D12TextureCubeArray* resource = static_cast<SD3D12TextureCubeArray*>(_resource);

	ClearUnorderAccessViewUINT(descriptorHeapRange->GetGpuDescriptorHandle(), uav->mCpuDescriptorHandle, resource->GetNativePtr(), _value);
}

void SD3D12CommandList::ClearUnorderAccessViewFloat(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDIBuffer* _resource, Math::SFColor _value) noexcept
{
	SD3D12DescriptorHeapRange* descriptorHeapRange = static_cast<SD3D12DescriptorHeapRange*>(_shaderVisibleUav);
	SD3D12UnorderedAccessView* uav = static_cast<SD3D12UnorderedAccessView*>(_uav);
	SD3D12Buffer* resource = static_cast<SD3D12Buffer*>(_resource);

	ClearUnorderAccessViewFloat(descriptorHeapRange->GetGpuDescriptorHandle(), uav->mCpuDescriptorHandle, resource->GetNativePtr(), _value);
}

void SD3D12CommandList::ClearUnorderAccessViewFloat(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDITexture1D* _resource, Math::SFColor _value) noexcept
{
	SD3D12DescriptorHeapRange* descriptorHeapRange = static_cast<SD3D12DescriptorHeapRange*>(_shaderVisibleUav);
	SD3D12UnorderedAccessView* uav = static_cast<SD3D12UnorderedAccessView*>(_uav);
	SD3D12Texture1D* resource = static_cast<SD3D12Texture1D*>(_resource);

	ClearUnorderAccessViewFloat(descriptorHeapRange->GetGpuDescriptorHandle(), uav->mCpuDescriptorHandle, resource->GetNativePtr(), _value);
}

void SD3D12CommandList::ClearUnorderAccessViewFloat(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDITexture1DArray* _resource, Math::SFColor _value) noexcept
{
	SD3D12DescriptorHeapRange* descriptorHeapRange = static_cast<SD3D12DescriptorHeapRange*>(_shaderVisibleUav);
	SD3D12UnorderedAccessView* uav = static_cast<SD3D12UnorderedAccessView*>(_uav);
	SD3D12Texture1DArray* resource = static_cast<SD3D12Texture1DArray*>(_resource);

	ClearUnorderAccessViewFloat(descriptorHeapRange->GetGpuDescriptorHandle(), uav->mCpuDescriptorHandle, resource->GetNativePtr(), _value);
}

void SD3D12CommandList::ClearUnorderAccessViewFloat(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDITexture2D* _resource, Math::SFColor _value) noexcept
{
	SD3D12DescriptorHeapRange* descriptorHeapRange = static_cast<SD3D12DescriptorHeapRange*>(_shaderVisibleUav);
	SD3D12UnorderedAccessView* uav = static_cast<SD3D12UnorderedAccessView*>(_uav);
	SD3D12Texture2D* resource = static_cast<SD3D12Texture2D*>(_resource);

	ClearUnorderAccessViewFloat(descriptorHeapRange->GetGpuDescriptorHandle(), uav->mCpuDescriptorHandle, resource->GetNativePtr(), _value);
}

void SD3D12CommandList::ClearUnorderAccessViewFloat(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDITexture2DArray* _resource, Math::SFColor _value) noexcept
{
	SD3D12DescriptorHeapRange* descriptorHeapRange = static_cast<SD3D12DescriptorHeapRange*>(_shaderVisibleUav);
	SD3D12UnorderedAccessView* uav = static_cast<SD3D12UnorderedAccessView*>(_uav);
	SD3D12Texture2DArray* resource = static_cast<SD3D12Texture2DArray*>(_resource);

	ClearUnorderAccessViewFloat(descriptorHeapRange->GetGpuDescriptorHandle(), uav->mCpuDescriptorHandle, resource->GetNativePtr(), _value);
}

void SD3D12CommandList::ClearUnorderAccessViewFloat(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDITexture3D* _resource, Math::SFColor _value) noexcept
{
	SD3D12DescriptorHeapRange* descriptorHeapRange = static_cast<SD3D12DescriptorHeapRange*>(_shaderVisibleUav);
	SD3D12UnorderedAccessView* uav = static_cast<SD3D12UnorderedAccessView*>(_uav);
	SD3D12Texture3D* resource = static_cast<SD3D12Texture3D*>(_resource);

	ClearUnorderAccessViewFloat(descriptorHeapRange->GetGpuDescriptorHandle(), uav->mCpuDescriptorHandle, resource->GetNativePtr(), _value);
}

void SD3D12CommandList::ClearUnorderAccessViewFloat(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDITextureCube* _resource, Math::SFColor _value) noexcept
{
	SD3D12DescriptorHeapRange* descriptorHeapRange = static_cast<SD3D12DescriptorHeapRange*>(_shaderVisibleUav);
	SD3D12UnorderedAccessView* uav = static_cast<SD3D12UnorderedAccessView*>(_uav);
	SD3D12TextureCube* resource = static_cast<SD3D12TextureCube*>(_resource);

	ClearUnorderAccessViewFloat(descriptorHeapRange->GetGpuDescriptorHandle(), uav->mCpuDescriptorHandle, resource->GetNativePtr(), _value);
}

void SD3D12CommandList::ClearUnorderAccessViewFloat(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDITextureCubeArray* _resource, Math::SFColor _value) noexcept
{
	SD3D12DescriptorHeapRange* descriptorHeapRange = static_cast<SD3D12DescriptorHeapRange*>(_shaderVisibleUav);
	SD3D12UnorderedAccessView* uav = static_cast<SD3D12UnorderedAccessView*>(_uav);
	SD3D12TextureCubeArray* resource = static_cast<SD3D12TextureCubeArray*>(_resource);

	ClearUnorderAccessViewFloat(descriptorHeapRange->GetGpuDescriptorHandle(), uav->mCpuDescriptorHandle, resource->GetNativePtr(), _value);
}

void SD3D12CommandList::UnorderAccessResourceBarrier(ID3D12Resource* _resourceNativePtr) noexcept
{
	D3D12_RESOURCE_BARRIER barrier;

	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_UAV;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.UAV.pResource = _resourceNativePtr;

	GetCommandListNativePtr()->ResourceBarrier(1, &barrier);
}

void SD3D12CommandList::TranstionResourceState(ID3D12Resource* _resourceNativePtr, ERDIResourceState _before, ERDIResourceState _after) noexcept
{
	D3D12_RESOURCE_BARRIER barrier;

	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = _resourceNativePtr;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = ConvertResourceStateToD3D12(_before);
	barrier.Transition.StateAfter = ConvertResourceStateToD3D12(_after);

	GetCommandListNativePtr()->ResourceBarrier(1, &barrier);
}

void SD3D12CommandList::ClearUnorderAccessViewUINT(D3D12_GPU_DESCRIPTOR_HANDLE _shaderVisibleViewGpuHandle, D3D12_CPU_DESCRIPTOR_HANDLE _cpuHandle, ID3D12Resource* _resourceNativePtr, Math::SUColor _value) noexcept
{
	GetCommandListNativePtr()->ClearUnorderedAccessViewUint(_shaderVisibleViewGpuHandle, _cpuHandle, _resourceNativePtr, _value.Color, 0, nullptr);
}

void SD3D12CommandList::ClearUnorderAccessViewFloat(D3D12_GPU_DESCRIPTOR_HANDLE _shaderVisibleViewGpuHandle, D3D12_CPU_DESCRIPTOR_HANDLE _cpuHandle, ID3D12Resource* _resourceNativePtr, Math::SFColor _value) noexcept
{
	GetCommandListNativePtr()->ClearUnorderedAccessViewFloat(_shaderVisibleViewGpuHandle, _cpuHandle, _resourceNativePtr, _value.Color, 0, nullptr);
}
