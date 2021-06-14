#pragma once

#include "RDI/Interface/RDICommandList.h"
#include "D3D/D3D12/Helper/D3D12Helper.h"

#include "Core/Misc/PreWindowsApi.h"
#include <d3d12.h>
#include "Core/Misc/PostWindowsApi.h"

class SD3D12Device;

class SD3D12CommandList : public IRDICommandList
{
public:
	void Init(ID3D12CommandAllocator* _commandAllocatorNativePtr, ID3D12GraphicsCommandList* _commandListNativePtr, SD3D12Device* _device) noexcept;
	void Clear() noexcept;

	ID3D12GraphicsCommandList* GetCommandListNativePtr() noexcept { return mCommandListNativePtr; }
	ID3D12CommandAllocator* GetCommandAllocatorNativePtr() noexcept { return mCommandAllocatorNativePtr; }

public:
	void ResetCommandAllocator() noexcept override;

	void ResetCommandList() noexcept override;
	void Close() noexcept override;

	void DrawIndexedInstanced(uint32_t _indexCount, uint32_t _instanceCount, uint32_t _startIndexLocation, uint32_t _baseVertexLocation) noexcept override;
	void Dispatch(uint32_t _threadGroupX, uint32_t _threadGroupY, uint32_t _threadGroupZ) noexcept override;

	void CopyBufferRegion(IRDIBuffer* _destBuffer, uint64_t _destOffset, IRDIBuffer* _srcBuffer, uint64_t _srcOffset, uint64_t _numByte) noexcept override;
	void CopyBuffer(IRDIBuffer* _destBuffer, IRDIBuffer* _srcBuffer) noexcept override;
	void CopyTexture1D(IRDITexture1D* _destTexture, IRDITexture1D* _srcTexture) noexcept override;
	void CopyTexture1DArray(IRDITexture1DArray* _destTexture, IRDITexture1DArray* _srcTexture) noexcept override;
	void CopyTexture2D(IRDITexture2D* _destTexture, IRDITexture2D* _srcTexture) noexcept override;
	void CopyTexture2DArray(IRDITexture2DArray* _destTexture, IRDITexture2DArray* _srcTexture) noexcept override;
	void CopyTexture3D(IRDITexture3D* _destTexture, IRDITexture3D* _srcTexture) noexcept override;
	void CopyTextureCube(IRDITextureCube* _destTexture, IRDITextureCube* _srcTexture) noexcept override;
	void CopyTextureCubeArray(IRDITextureCubeArray* _destTexture, IRDITextureCubeArray* _srcTexture) noexcept override;

	void UnorderAccessResourceBarrier(IRDIBuffer* _resource) noexcept override;
	void UnorderAccessResourceBarrier(IRDITexture1D* _resource) noexcept override;
	void UnorderAccessResourceBarrier(IRDITexture1DArray* _resource) noexcept override;
	void UnorderAccessResourceBarrier(IRDITexture2D* _resource) noexcept override;
	void UnorderAccessResourceBarrier(IRDITexture2DArray* _resource) noexcept override;
	void UnorderAccessResourceBarrier(IRDITexture3D* _resource) noexcept override;
	void UnorderAccessResourceBarrier(IRDITextureCube* _resource) noexcept override;
	void UnorderAccessResourceBarrier(IRDITextureCubeArray* _resource) noexcept override;

	void TranstionResourceState(IRDIBuffer* _resource, ERDIResourceState _before, ERDIResourceState _after) noexcept override;
	void TranstionResourceState(IRDITexture1D* _resource, ERDIResourceState _before, ERDIResourceState _after) noexcept override;
	void TranstionResourceState(IRDITexture1DArray* _resource, ERDIResourceState _before, ERDIResourceState _after) noexcept override;
	void TranstionResourceState(IRDITexture2D* _resource, ERDIResourceState _before, ERDIResourceState _after) noexcept override;
	void TranstionResourceState(IRDITexture2DArray* _resource, ERDIResourceState _before, ERDIResourceState _after) noexcept override;
	void TranstionResourceState(IRDITexture3D* _resource, ERDIResourceState _before, ERDIResourceState _after) noexcept override;
	void TranstionResourceState(IRDITextureCube* _resource, ERDIResourceState _before, ERDIResourceState _after) noexcept override;
	void TranstionResourceState(IRDITextureCubeArray* _resource, ERDIResourceState _before, ERDIResourceState _after) noexcept override;

	void IASetPrimitiveTopology(ERDIPrimitiveTopology _primitiveTopology) noexcept override;
	void IASetIndexBuffer(IRDIIndexBufferView* _ibv) noexcept override;
	void IASetVertexBuffer(uint32_t _slotIndex, uint32_t _bufferCount, IRDIVertexBufferView* const* _vbv) noexcept override;
	void RSSetViewports(uint32_t _numViewports, const Math::SFloatBox* _viewports) noexcept override;
	void RSSetScissorRects(uint32_t _numRects, const Math::SIntRect* _rects) noexcept override;
	void OMSetRenderTargets(uint32_t _renderTargetCount, IRDIRenderTargetView* const* _rtvs, IRDIDepthStencilView* _dsv) noexcept override;
	void OMSetBlendFactor(Math::SFColor _blendFactor) noexcept override;
	void OMSetStencilRef(uint32_t _stencilRef) noexcept override;

	void SetGraphicsPipelineState(IRDIGraphicsPipelineState* _pipelineState) noexcept override;
	void SetComputePipelineState(IRDIComputePipelineState* _pipelineState) noexcept override;

	void SetGraphicsRootSignature(IRDIRootSignature* _rootSignature) noexcept override;
	void SetComputeRootSignature(IRDIRootSignature* _rootSignature) noexcept override;

	void SetGraphicsRootConstantBuffer(uint32_t _rootParameterIndex, IRDIBuffer* _buffer, uint64_t _offset) noexcept override;
	void SetComputeRootConstantBuffer(uint32_t _rootParameterIndex, IRDIBuffer* _buffer, uint64_t _offset) noexcept override;
	void SetGraphicsRootDescriptorTable(uint32_t _rootParameterIndex, IRDIDescriptorHeapRange* _descriptorRange) noexcept override;
	void SetComputeRootDescriptorTable(uint32_t _rootParameterIndex, IRDIDescriptorHeapRange* _descriptorRange) noexcept override;
	void SetGraphicsRootSamplerDescriptorTable(uint32_t _rootParameterIndex, IRDISamplerHeapRange* _samplerRange) noexcept override;
	void SetComputeRootSamplerDescriptorTable(uint32_t _rootParameterIndex, IRDISamplerHeapRange* _samplerRange) noexcept override;

	void ClearDepthStencilView(IRDIDepthStencilView* _dsv, ERDIClearFlag _clearFlag, float _depth, uint8_t _stencil) noexcept override;
	void ClearRenderTargetView(IRDIRenderTargetView* _rtv, Math::SFColor _color) noexcept override;

	void ClearUnorderAccessViewUINT(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDIBuffer* _resource, Math::SUColor _value) noexcept override;
	void ClearUnorderAccessViewUINT(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDITexture1D* _resource, Math::SUColor _value) noexcept override;
	void ClearUnorderAccessViewUINT(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDITexture1DArray* _resource, Math::SUColor _value) noexcept override;
	void ClearUnorderAccessViewUINT(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDITexture2D* _resource, Math::SUColor _value) noexcept override;
	void ClearUnorderAccessViewUINT(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDITexture2DArray* _resource, Math::SUColor _value) noexcept override;
	void ClearUnorderAccessViewUINT(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDITexture3D* _resource, Math::SUColor _value) noexcept override;
	void ClearUnorderAccessViewUINT(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDITextureCube* _resource, Math::SUColor _value) noexcept override;
	void ClearUnorderAccessViewUINT(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDITextureCubeArray* _resource, Math::SUColor _value) noexcept override;

	void ClearUnorderAccessViewFloat(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDIBuffer* _resource, Math::SFColor _value) noexcept override;
	void ClearUnorderAccessViewFloat(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDITexture1D* _resource, Math::SFColor _value) noexcept override;
	void ClearUnorderAccessViewFloat(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDITexture1DArray* _resource, Math::SFColor _value) noexcept override;
	void ClearUnorderAccessViewFloat(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDITexture2D* _resource, Math::SFColor _value) noexcept override;
	void ClearUnorderAccessViewFloat(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDITexture2DArray* _resource, Math::SFColor _value) noexcept override;
	void ClearUnorderAccessViewFloat(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDITexture3D* _resource, Math::SFColor _value) noexcept override;
	void ClearUnorderAccessViewFloat(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDITextureCube* _resource, Math::SFColor _value) noexcept override;
	void ClearUnorderAccessViewFloat(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDITextureCubeArray* _resource, Math::SFColor _value) noexcept override;

private:
	void UnorderAccessResourceBarrier(ID3D12Resource* _resourceNativePtr) noexcept;
	void TranstionResourceState(ID3D12Resource* _resourceNativePtr, ERDIResourceState _before, ERDIResourceState _after) noexcept;
	void ClearUnorderAccessViewUINT(D3D12_GPU_DESCRIPTOR_HANDLE _shaderVisibleViewGpuHandle, D3D12_CPU_DESCRIPTOR_HANDLE _cpuHandle, ID3D12Resource* _resourceNativePtr, Math::SUColor _value) noexcept;
	void ClearUnorderAccessViewFloat(D3D12_GPU_DESCRIPTOR_HANDLE _shaderVisibleViewGpuHandle, D3D12_CPU_DESCRIPTOR_HANDLE _cpuHandle, ID3D12Resource* _resourceNativePtr, Math::SFColor _value) noexcept;

private:
	ID3D12CommandAllocator* mCommandAllocatorNativePtr = nullptr;
	ID3D12GraphicsCommandList* mCommandListNativePtr = nullptr;

	SD3D12Device* mDevice = nullptr;

	D3D12_VERTEX_BUFFER_VIEW mViewDescCache[D3D12_VERTEX_BUFFER_SLOT_COUNT] = {};
	D3D12_VIEWPORT mViewportsCache[D3D12_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE] = {};
	D3D12_RECT mScissorRectCache[D3D12_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE] = {};
	D3D12_CPU_DESCRIPTOR_HANDLE mRtvDescriptorHandlesCache[D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT] = {};
};
