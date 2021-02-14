#pragma once

#include "Core/Math/Type.h"

#include <stdint.h>

enum class ERDIResourceState;
enum class ERDIPrimitiveTopology;

__interface IRDIRootSignature;
__interface IRDIDescriptorHeapRange;
__interface IRDISamplerHeapRange;
__interface IRDIGraphicsPipelineState;
__interface IRDIComputePipelineState;

__interface IRDIIndexBufferView;
__interface IRDIVertexBufferView;
__interface IRDIRenderTargetView;
__interface IRDIDepthStencilView;
__interface IRDIShaderResourceView;
__interface IRDIUnorderedAccessView;

__interface IRDIResource;
__interface IRDIBuffer;
__interface IRDITexture1D;
__interface IRDITexture1DArray;
__interface IRDITexture2D;
__interface IRDITexture2DArray;
__interface IRDITexture3D;
__interface IRDITextureCube;
__interface IRDITextureCubeArray;
__interface IRDISampler;

enum class ERDIClearFlag;

__interface IRDICommandList
{
	void ResetCommandAllocator() noexcept;

	void ResetCommandList() noexcept;
	void Close() noexcept;

	void CopyBufferRegion(IRDIBuffer* _destBuffer, uint64_t _destOffset, IRDIBuffer* _srcBuffer, uint64_t _srcOffset, uint64_t _numByte) noexcept;
	void CopyBuffer(IRDIBuffer* _destBuffer, IRDIBuffer* _srcBuffer) noexcept;
	void CopyTexture1D(IRDITexture1D* _destTexture, IRDITexture1D* _srcTexture) noexcept;
	void CopyTexture1DArray(IRDITexture1DArray* _destTexture, IRDITexture1DArray* _srcTexture) noexcept;
	void CopyTexture2D(IRDITexture2D* _destTexture, IRDITexture2D* _srcTexture) noexcept;
	void CopyTexture2DArray(IRDITexture2DArray* _destTexture, IRDITexture2DArray* _srcTexture) noexcept;
	void CopyTexture3D(IRDITexture3D* _destTexture, IRDITexture3D* _srcTexture) noexcept;
	void CopyTextureCube(IRDITextureCube* _destTexture, IRDITextureCube* _srcTexture) noexcept;
	void CopyTextureCubeArray(IRDITextureCubeArray* _destTexture, IRDITextureCubeArray* _srcTexture) noexcept;

	void UnorderAccessResourceBarrier(IRDIBuffer* _resource) noexcept;
	void UnorderAccessResourceBarrier(IRDITexture1D* _resource) noexcept;
	void UnorderAccessResourceBarrier(IRDITexture1DArray* _resource) noexcept;
	void UnorderAccessResourceBarrier(IRDITexture2D* _resource) noexcept;
	void UnorderAccessResourceBarrier(IRDITexture2DArray* _resource) noexcept;
	void UnorderAccessResourceBarrier(IRDITexture3D* _resource) noexcept;
	void UnorderAccessResourceBarrier(IRDITextureCube* _resource) noexcept;
	void UnorderAccessResourceBarrier(IRDITextureCubeArray* _resource) noexcept;

	void TranstionResourceState(IRDIBuffer* _resource, ERDIResourceState _before, ERDIResourceState _after) noexcept;
	void TranstionResourceState(IRDITexture1D* _resource, ERDIResourceState _before, ERDIResourceState _after) noexcept;
	void TranstionResourceState(IRDITexture1DArray* _resource, ERDIResourceState _before, ERDIResourceState _after) noexcept;
	void TranstionResourceState(IRDITexture2D* _resource, ERDIResourceState _before, ERDIResourceState _after) noexcept;
	void TranstionResourceState(IRDITexture2DArray* _resource, ERDIResourceState _before, ERDIResourceState _after) noexcept;
	void TranstionResourceState(IRDITexture3D* _resource, ERDIResourceState _before, ERDIResourceState _after) noexcept;
	void TranstionResourceState(IRDITextureCube* _resource, ERDIResourceState _before, ERDIResourceState _after) noexcept;
	void TranstionResourceState(IRDITextureCubeArray* _resource, ERDIResourceState _before, ERDIResourceState _after) noexcept;

	void IASetPrimitiveTopology(ERDIPrimitiveTopology _primitiveTopology);
	void IASetIndexBuffer(IRDIIndexBufferView* _ibv) noexcept;
	void IASetVertexBuffer(uint32_t _slotIndex, uint32_t _bufferCount, IRDIVertexBufferView* const* _vbv) noexcept;
	void RSSetViewports(uint32_t _numViewports, const Math::SFloatBox* _viewports) noexcept;
	void RSSetScissorRects(uint32_t _numRects, const Math::SIntRect* _rects) noexcept;
	void OMSetRenderTargets(uint32_t _renderTargetCount, IRDIRenderTargetView* const* _rtvs, IRDIDepthStencilView* _dsv) noexcept;
	void OMSetBlendFactor(Math::SFColor _blendFactor) noexcept;
	void OMSetStencilRef(uint32_t _stencilRef) noexcept;

	void SetGraphicsPipelineState(IRDIGraphicsPipelineState* _pipelineState) noexcept;
	void SetComputePipelineState(IRDIComputePipelineState* _pipelineState) noexcept;

	void SetGraphicsRootSignature(IRDIRootSignature* _rootSignature) noexcept;
	void SetComputeRootSignature(IRDIRootSignature* _rootSignature) noexcept;

	void SetGraphicsRootConstantBuffer(uint32_t _rootParameterIndex, IRDIBuffer* _buffer, uint64_t _offset) noexcept;
	void SetComputeRootConstantBuffer(uint32_t _rootParameterIndex, IRDIBuffer* _buffer, uint64_t _offset) noexcept;
	void SetGraphicsRootDescriptorTable(uint32_t _rootParameterIndex, IRDIDescriptorHeapRange* _descriptorRange) noexcept;
	void SetComputeRootDescriptorTable(uint32_t _rootParameterIndex, IRDIDescriptorHeapRange* _descriptorRange) noexcept;
	void SetGraphicsRootSamplerDescriptorTable(uint32_t _rootParameterIndex, IRDISamplerHeapRange* _samplerRange) noexcept;
	void SetComputeRootSamplerDescriptorTable(uint32_t _rootParameterIndex, IRDISamplerHeapRange* _samplerRange) noexcept;

	void DrawIndexedInstanced(uint32_t _indexCount, uint32_t _instanceCount, uint32_t _startIndexLocation, uint32_t _baseVertexLocation) noexcept;
	void Dispatch(uint32_t _threadGroupX, uint32_t _threadGroupY, uint32_t _threadGroupZ) noexcept;

	void ClearDepthStencilView(IRDIDepthStencilView* _dsv, ERDIClearFlag _clearFlag, float _depth, uint8_t _stencil) noexcept;
	void ClearRenderTargetView(IRDIRenderTargetView* _rtv, Math::SFColor _color) noexcept;
	
	void ClearUnorderAccessViewUINT(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDIBuffer* _resource, Math::SUColor _value) noexcept;
	void ClearUnorderAccessViewUINT(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDITexture1D* _resource, Math::SUColor _value) noexcept;
	void ClearUnorderAccessViewUINT(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDITexture1DArray* _resource, Math::SUColor _value) noexcept;
	void ClearUnorderAccessViewUINT(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDITexture2D* _resource, Math::SUColor _value) noexcept;
	void ClearUnorderAccessViewUINT(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDITexture2DArray* _resource, Math::SUColor _value) noexcept;
	void ClearUnorderAccessViewUINT(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDITexture3D* _resource, Math::SUColor _value) noexcept;
	void ClearUnorderAccessViewUINT(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDITextureCube* _resource, Math::SUColor _value) noexcept;
	void ClearUnorderAccessViewUINT(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDITextureCubeArray* _resource, Math::SUColor _value) noexcept;

	void ClearUnorderAccessViewFloat(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDIBuffer* _resource, Math::SFColor _value) noexcept;
	void ClearUnorderAccessViewFloat(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDITexture1D* _resource, Math::SFColor _value) noexcept;
	void ClearUnorderAccessViewFloat(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDITexture1DArray* _resource, Math::SFColor _value) noexcept;
	void ClearUnorderAccessViewFloat(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDITexture2D* _resource, Math::SFColor _value) noexcept;
	void ClearUnorderAccessViewFloat(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDITexture2DArray* _resource, Math::SFColor _value) noexcept;
	void ClearUnorderAccessViewFloat(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDITexture3D* _resource, Math::SFColor _value) noexcept;
	void ClearUnorderAccessViewFloat(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDITextureCube* _resource, Math::SFColor _value) noexcept;
	void ClearUnorderAccessViewFloat(IRDIDescriptorHeapRange* _shaderVisibleUav, IRDIUnorderedAccessView* _uav, IRDITextureCubeArray* _resource, Math::SFColor _value) noexcept;
};
