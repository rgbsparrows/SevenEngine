#pragma once

#include <string>
#include <vector>
#include <stdint.h>

namespace D3D12APIWarp_Impl
{
	uint32_t D3D12CreateDevice_D3D12Impl(void* _adpater, void** _device) noexcept;
	uint32_t D3D12CreateSwapChain_D3D12Impl(void* _dxgiFactory, void* _device, void* _desc, void** _swapChain) noexcept;
	uint32_t D3D12GetDebugInterface_D3D12Impl(void** _debugInterface) noexcept;

#pragma region ID3D12Debug
	void D3D12EnableDebugLayer_D3D12Impl(void* _debugInterface) noexcept;
	void D3D12SetEnableGPUBasedValidation_D3D12Impl(void* _debugInterface) noexcept;
#pragma endregion

#pragma region ID3D12Device
	uint32_t D3D12CreateCommandQueue_D3D12Impl(void* _device, void** _commandQueue) noexcept;
	uint32_t D3D12CreateFence_D3D12Impl(void* _device, uint64_t _initialValue, void** _fence) noexcept;
	uint32_t D3D12CreateDescriptorHeap_D3D12Impl(void* _device, int32_t _descriptorType, bool _isShaderVisible, uint32_t _descriptorCount, void** _descriptorHeap) noexcept;
	uint32_t D3D12CreateCommandList_D3D12Impl(void* _device, void** _commandAllocator, void** _commandList) noexcept;
	void D3D12CreateRenderTargetView_D3D12Impl(void* _resource, const void* _renderTargetViewDesc, uint64_t _cpuDescriptorHandle, void* _device) noexcept;
	void D3D12CreateDepthStencilView_D3D12Impl(void* _resource, const void* _depthStencilViewDesc, uint64_t _cpuDescriptorHandle, void* _device) noexcept;
	void D3D12CreateShaderResourceView_D3D12Impl(void* _resource, const void* _shaderResourceViewDesc, uint64_t _cpuDescriptorHandle, void* _device) noexcept;
	void D3D12CreateUnorderedAccessView_D3D12Impl(void* _resource, void* _counterResource, const void* _unorderedAccessViewDesc, uint64_t _cpuDescriptorHandle, void* _device) noexcept;
	void D3D12CreateSamplerView_D3D12Impl(const void* _samplerViewDesc, uint64_t _cpuDescriptorHandle, void* _device) noexcept;
	uint32_t D3D12CreateCommittedResource_D3D12Impl(void* _device, const void* _heapProperties, int32_t _heapFlags, const void* _desc, int32_t _initialResourceState, const void* _optimizedClearValue, void** _resource) noexcept;
	uint32_t D3D12CreateRootSignature_D3D12Impl(const void* _rootSignatureBlob, size_t _blobSize, void* _device, void** _rootSignature) noexcept;
	uint32_t D3D12CreateGraphicsPipelineState_D3D12Impl(const void* _desc, void* _device, void** _pipelineState) noexcept;
	uint32_t D3D12CreateComputePipelineState_D3D12Impl(const void* _desc, void* _device, void** _pipelineState) noexcept;
	void D3D12CopyDescriptorsSimple_D3D12Impl(void* _device, uint32_t _numDescriptors, size_t _destDescriptorRangeStart, size_t _srcDescriptorRangeStart, int32_t _descriptorType) noexcept;
	void D3D12GetDescriptorHandleIncrementSize_D3D12Impl(void* _device, int32_t _descriptorType, uint32_t* _incrementSize) noexcept;
#pragma endregion

#pragma region ID3D12CommandList

	uint32_t D3D12ResetCommandAllocator_D3D12Impl(void* _commandAllocator) noexcept;
	uint32_t D3D12ResetCommandList_D3D12Impl(void* _commandList, void* _commandAllocator) noexcept;
	uint32_t D3D12CloseCommandList_D3D12Impl(void* _commandList) noexcept;
	void D3D12SetDescriptorHeaps(void* _commandList, uint32_t _descriptorHeapCount, void* const* _descriptorHeaps) noexcept;
	void D3D12DrawIndexedInstanced_D3D12Impl(void* _commandList, uint32_t _indexCountPerInstance, uint32_t _instanceCount, uint32_t _startIndexLocation, int32_t _baseVertexLocation, uint32_t _startInstanceLocation) noexcept;
	void D3D12Dispatch_D3D12Impl(void* _commandList, uint32_t _threadGroupCountX, uint32_t _threadGroupCountY, uint32_t _threadGroupCountZ) noexcept;
	void D3D12CopyBufferRegion_D3D12Impl(void* _commandList, void* _destBuffer, uint64_t _destOffset, void* _srcBuffer, uint64_t _srcOffset, uint64_t _numBytes) noexcept;
	void D3D12CopyResource_D3D12Impl(void* _commandList, void* _destResource, void* _srcResource) noexcept;
	void D3D12ResourceBarrier_D3D12Impl(void* _commandList, uint32_t _numBarriers, const void* _barriers) noexcept;
	void D3D12IASetPrimitiveTopology_D3D12Impl(void* _commandList, uint32_t _primitiveTopology) noexcept;
	void D3D12IASetIndexBuffer_D3D12Impl(void* _commandList, const void* _indexBufferView) noexcept;
	void D3D12IASetVertexBuffers_D3D12Impl(void* _commandList, uint32_t _startSlot, uint32_t _numViews, const void* _views) noexcept;
	void D3D12RSSetViewports_D3D12Impl(void* _commandList, uint32_t _numViewports, const void* _viewports) noexcept;
	void D3D12RSSetScissorRects_D3D12Impl(void* _commandList, uint32_t _numRects, const void* _rects) noexcept;
	void D3D12OMSetRenderTargets_D3D12Impl(void* _commandList, uint32_t _numRenderTargetDescriptors, const void* _renderTargetDescriptors, bool _rtsSingleHandleToDescriptorRange, const void* _depthStencilDescriptor) noexcept;
	void D3D12OMSetBlendFactor_D3D12Impl(void* _commandList, const float* _blendFactor) noexcept;
	void D3D12OMSetStencilRef_D3D12Impl(void* _commandList, uint32_t _stencilRef) noexcept;
	void D3D12SetPipelineState_D3D12Impl(void* _commandList, void* _pipelineState) noexcept;
	void D3D12SetGraphicsRootSignature_D3D12Impl(void* _commandList, void* _rootSignature) noexcept;
	void D3D12SetComputeRootSignature_D3D12Impl(void* _commandList, void* _rootSignature) noexcept;
	void D3D12SetGraphicsRootConstantBufferView_D3D12Impl(void* _commandList, uint32_t _rootParameterIndex, uint64_t _bufferLocation) noexcept;
	void D3D12SetComputeRootConstantBufferView_D3D12Impl(void* _commandList, uint32_t _rootParameterIndex, uint64_t _bufferLocation) noexcept;
	void D3D12SetGraphicsRootDescriptorTable_D3D12Impl(void* _commandList, uint32_t _rootParameterIndex, uint64_t _baseDescriptor) noexcept;
	void D3D12SetComputeRootDescriptorTable_D3D12Impl(void* _commandList, uint32_t _rootParameterIndex, uint64_t _baseDescriptor) noexcept;
	void D3D12ClearDepthStencilView_D3D12Impl(void* _commandList, size_t _depthStencilView, int32_t _clearFlags, float _depth, uint8_t _stencil, uint32_t _numRects, const void* _rects) noexcept;
	void D3D12ClearRenderTargetView_D3D12Impl(void* _commandList, size_t _renderTargetView, const float* _colorRGBA, uint32_t _numRects, const void* _rects) noexcept;
	void D3D12ClearUnorderedAccessViewUint_D3D12Impl(void* _commandList, uint64_t _shaderVisibleViewGpuHandle, size_t _cpuHandle, void* _resource, const uint32_t* _value, uint32_t _numRect, const void* _rects) noexcept;
	void D3D12ClearUnorderedAccessViewFloat_D3D12Impl(void* _commandList, uint64_t _shaderVisibleViewGpuHandle, size_t _cpuHandle, void* _resource, const float* _value, uint32_t _numRect, const void* _rects) noexcept;

#pragma endregion

#pragma region ID3D12CommandQueue ID3D12Fence
	void D3D12ExecuteCommandLists_D3D12Impl(void* _commandQueue, uint32_t _commandListCount, void* const* _commandLists) noexcept;
	uint32_t D3D12Signal_D3D12Impl(void* _commandQueue, void* _fence, uint64_t _value) noexcept;
	uint64_t D3D12GetCompletedValue_D3D12Impl(void* _fence) noexcept;
	uint32_t D3D12WaitUntilFenceComplete_D3D12Impl(void* _fence, uint64_t _fenceValue) noexcept;
#pragma endregion

#pragma region ID3D12Resource
	void D3D12GetGPUVirtualAddress_D3D12Impl(void* _resource, uint64_t* _gpuVirtualAddress) noexcept;
#pragma endregion

#pragma region ID3D12DescriptorHeap
	void D3D12GetCPUDescriptorHandleForHeapStart_D3D12Impl(void* _descriptorHeap, size_t* _startCpuDescriptorHandle) noexcept;
	void D3D12GetGPUDescriptorHandleForHeapStart_D3D12Impl(void* _descriptorHeap, uint64_t* _startGpuDescriptorHandle) noexcept;
#pragma endregion

#pragma region ID3D12RootSignature
	uint32_t D3D12SerializeRootSignature_D3D12Impl(const void* _desc, int32_t _verision, std::vector<uint8_t>& _serlizedBlob, std::vector<uint8_t> errorBlob) noexcept;
#pragma endregion

#pragma region ID3D12PipelineState
	uint32_t D3D12GetPipelineStateCachedBlob(void* _pipelineState, std::vector<uint8_t>& _cachedBlob) noexcept;
#pragma endregion

#pragma region IDXGISwapChain
	uint32_t D3D12SwapChainGetBuffer_D3D12Impl(void* _swapChain, uint32_t _index, void** _renderTaget) noexcept;
#pragma endregion
}
