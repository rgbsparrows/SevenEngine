#include "D3D12ImplWarper.h"

#include <d3d12.h>
#include <stdint.h>
#include <dxgi1_6.h>
#include <windows.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

using SD3D12Debug = ID3D12Debug;
using SD3D12NativeDXGIFactoryType = IDXGIFactory;
using SD3D12NativeDebugControllerType = ID3D12Debug;
using SD3D12NativeSwapChainType = IDXGISwapChain;
using SD3D12NativeAdapterType = IDXGIAdapter;
using SD3D12NativeOutputType = IDXGIOutput;
using SD3D12NativeDeviceType = ID3D12Device;
using SD3D12NativeCommandQueueType = ID3D12CommandQueue;
using SD3D12NativeFenceType = ID3D12Fence;
using SD3D12NativeCommandAllocatorType = ID3D12CommandAllocator;
using SD3D12NativeCommandListType = ID3D12GraphicsCommandList;
using SD3D12NativeResourceType = ID3D12Resource;
using SD3D12NativePipelineStateType = ID3D12PipelineState;
using SD3D12NativeRootSignatureType = ID3D12RootSignature;
using SD3D12NativeDescriptorHeapType = ID3D12DescriptorHeap;

namespace D3D12APIWarp_Impl
{
	uint32_t D3D12CreateDevice_D3D12Impl(void* _adpater, void** _device) noexcept
	{
		SD3D12NativeAdapterType* adapter = static_cast<SD3D12NativeAdapterType*>(_adpater);
		ID3D12Device* device = nullptr;
		HRESULT res = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device));
		*_device = static_cast<SD3D12NativeDeviceType*>(device);
		return res;
	}

	uint32_t D3D12CreateSwapChain_D3D12Impl(void* _dxgiFactory, void* _device, void* _desc, void** _swapChain) noexcept
	{
		SD3D12NativeDXGIFactoryType* dxgiFactory = static_cast<SD3D12NativeDXGIFactoryType*>(_dxgiFactory);
		IDXGISwapChain* swapChain;
		HRESULT res = dxgiFactory->CreateSwapChain(static_cast<SD3D12NativeDeviceType*>(_device), static_cast<DXGI_SWAP_CHAIN_DESC*>(_desc), &swapChain);
		*_swapChain = static_cast<SD3D12NativeSwapChainType*>(swapChain);
		return res;
	}

	uint32_t D3D12GetDebugInterface_D3D12Impl(void** _debugInterface) noexcept
	{
		ID3D12Debug* debug = nullptr;
		HRESULT res = D3D12GetDebugInterface(IID_PPV_ARGS(&debug));

		*_debugInterface = static_cast<SD3D12Debug*>(debug);
		return res;
	}

	void D3D12EnableDebugLayer_D3D12Impl(void* _debugInterface) noexcept
	{
		SD3D12Debug* debug = static_cast<SD3D12Debug*>(_debugInterface);
		debug->EnableDebugLayer();
	}

	void D3D12SetEnableGPUBasedValidation_D3D12Impl(void* _debugInterface) noexcept
	{
		SD3D12Debug* debug = static_cast<SD3D12Debug*>(_debugInterface);

		static_cast<ID3D12Debug3*>(debug)->SetEnableGPUBasedValidation(TRUE);
	}

	uint32_t D3D12CreateCommandQueue_D3D12Impl(void* _device, void** _commandQueue) noexcept
	{
		SD3D12NativeDeviceType* device = static_cast<SD3D12NativeDeviceType*>(_device);
		D3D12_COMMAND_QUEUE_DESC desc{ D3D12_COMMAND_LIST_TYPE_DIRECT , 0, D3D12_COMMAND_QUEUE_FLAG_NONE, 0 };
		ID3D12CommandQueue* commandQueue = nullptr;
		HRESULT res = device->CreateCommandQueue(&desc, IID_PPV_ARGS(&commandQueue));
		*_commandQueue = static_cast<SD3D12NativeCommandQueueType*>(commandQueue);
		return res;
	}

	uint32_t D3D12CreateFence_D3D12Impl(void* _device, uint64_t _initialValue, void** _fence) noexcept
	{
		SD3D12NativeDeviceType* device = static_cast<SD3D12NativeDeviceType*>(_device);
		ID3D12Fence* fence = nullptr;
		HRESULT res = device->CreateFence(_initialValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
		*_fence = static_cast<SD3D12NativeFenceType*>(fence);
		return res;
	}

	uint32_t D3D12CreateDescriptorHeap_D3D12Impl(void* _device, int32_t _descriptorType, bool _isShaderVisible, uint32_t _descriptorCount, void** _descriptorHeap) noexcept
	{
		SD3D12NativeDeviceType* device = static_cast<SD3D12NativeDeviceType*>(_device);
		D3D12_DESCRIPTOR_HEAP_DESC descriptorHeadDesc{
			static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(_descriptorType),
			_descriptorCount,
			_isShaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
			0
		};
		ID3D12DescriptorHeap* descriptorHeap = nullptr;
		HRESULT res = device->CreateDescriptorHeap(&descriptorHeadDesc, IID_PPV_ARGS(&descriptorHeap));
		*_descriptorHeap = static_cast<SD3D12NativeDescriptorHeapType*>(descriptorHeap);
		return res;
	}

	uint32_t D3D12CreateCommandList_D3D12Impl(void* _device, void** _commandAllocator, void** _commandList) noexcept
	{
		SD3D12NativeDeviceType* device = static_cast<SD3D12NativeDeviceType*>(_device);
		ID3D12CommandAllocator* commandAllocator = nullptr;
		ID3D12GraphicsCommandList* commandList = nullptr;
		HRESULT res = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
		if (FAILED(res))
			return res;
		res = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator, nullptr, IID_PPV_ARGS(&commandList));
		*_commandAllocator = static_cast<SD3D12NativeCommandAllocatorType*>(commandAllocator);
		*_commandList = static_cast<SD3D12NativeCommandListType*>(commandList);
		return res;
	}

	void D3D12CreateRenderTargetView_D3D12Impl(void* _resource, const void* _renderTargetViewDesc, uint64_t _cpuDescriptorHandle, void* _device) noexcept
	{
		SD3D12NativeResourceType* resource = static_cast<SD3D12NativeResourceType*>(_resource);
		SD3D12NativeDeviceType* device = static_cast<SD3D12NativeDeviceType*>(_device);

		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
		cpuHandle.ptr = static_cast<SIZE_T>(_cpuDescriptorHandle);

		device->CreateRenderTargetView(resource, static_cast<const D3D12_RENDER_TARGET_VIEW_DESC*>(_renderTargetViewDesc), cpuHandle);
	}

	void D3D12CreateDepthStencilView_D3D12Impl(void* _resource, const void* _depthStencilViewDesc, uint64_t _cpuDescriptorHandle, void* _device) noexcept
	{
		SD3D12NativeResourceType* resource = static_cast<SD3D12NativeResourceType*>(_resource);
		SD3D12NativeDeviceType* device = static_cast<SD3D12NativeDeviceType*>(_device);

		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
		cpuHandle.ptr = static_cast<SIZE_T>(_cpuDescriptorHandle);

		device->CreateDepthStencilView(resource, static_cast<const D3D12_DEPTH_STENCIL_VIEW_DESC*>(_depthStencilViewDesc), cpuHandle);
	}

	void D3D12CreateShaderResourceView_D3D12Impl(void* _resource, const void* _shaderResourceViewDesc, uint64_t _cpuDescriptorHandle, void* _device) noexcept
	{
		SD3D12NativeResourceType* resource = static_cast<SD3D12NativeResourceType*>(_resource);
		SD3D12NativeDeviceType* device = static_cast<SD3D12NativeDeviceType*>(_device);

		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
		cpuHandle.ptr = static_cast<SIZE_T>(_cpuDescriptorHandle);

		device->CreateShaderResourceView(resource, static_cast<const D3D12_SHADER_RESOURCE_VIEW_DESC*>(_shaderResourceViewDesc), cpuHandle);
	}

	void D3D12CreateUnorderedAccessView_D3D12Impl(void* _resource, void* _counterResource, const void* _unorderedAccessViewDesc, uint64_t _cpuDescriptorHandle, void* _device) noexcept
	{
		SD3D12NativeResourceType* resource = static_cast<SD3D12NativeResourceType*>(_resource);
		SD3D12NativeResourceType* counterResource = static_cast<SD3D12NativeResourceType*>(_counterResource);
		SD3D12NativeDeviceType* device = static_cast<SD3D12NativeDeviceType*>(_device);

		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
		cpuHandle.ptr = static_cast<SIZE_T>(_cpuDescriptorHandle);

		device->CreateUnorderedAccessView(resource, counterResource, static_cast<const D3D12_UNORDERED_ACCESS_VIEW_DESC*>(_unorderedAccessViewDesc), cpuHandle);
	}

	void D3D12CreateSamplerView_D3D12Impl(const void* _samplerViewDesc, uint64_t _cpuDescriptorHandle, void* _device) noexcept
	{
		SD3D12NativeDeviceType* device = static_cast<SD3D12NativeDeviceType*>(_device);

		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
		cpuHandle.ptr = static_cast<SIZE_T>(_cpuDescriptorHandle);

		device->CreateSampler(static_cast<const D3D12_SAMPLER_DESC*>(_samplerViewDesc), cpuHandle);
	}

	uint32_t D3D12CreateCommittedResource_D3D12Impl(void* _device, const void* _heapProperties, int32_t _heapFlags, const void* _desc, int32_t _initialResourceState, const void* _optimizedClearValue, void** _resource) noexcept
	{
		SD3D12NativeDeviceType* device = static_cast<SD3D12NativeDeviceType*>(_device);
		ID3D12Resource* resource = nullptr;
		HRESULT res = device->CreateCommittedResource(static_cast<const D3D12_HEAP_PROPERTIES*>(_heapProperties), static_cast<D3D12_HEAP_FLAGS>(_heapFlags), static_cast<const D3D12_RESOURCE_DESC*>(_desc), static_cast<D3D12_RESOURCE_STATES>(_initialResourceState), static_cast<const D3D12_CLEAR_VALUE*>(_optimizedClearValue), IID_PPV_ARGS(&resource));
		*_resource = static_cast<SD3D12NativeResourceType*>(resource);
		return res;
	}

	uint32_t D3D12CreateRootSignature_D3D12Impl(const void* _rootSignatureBlob, size_t _blobSize, void* _device, void** _rootSignature) noexcept
	{
		SD3D12NativeDeviceType* device = static_cast<SD3D12NativeDeviceType*>(_device);
		ID3D12RootSignature* rootSignature = nullptr;
		HRESULT res = device->CreateRootSignature(0, _rootSignatureBlob, _blobSize, IID_PPV_ARGS(&rootSignature));
		*_rootSignature = static_cast<SD3D12NativeRootSignatureType*>(rootSignature);
		return res;
	}

	uint32_t D3D12CreateGraphicsPipelineState_D3D12Impl(const void* _desc, void* _device, void** _pipelineState) noexcept
	{
		SD3D12NativeDeviceType* device = static_cast<SD3D12NativeDeviceType*>(_device);
		ID3D12PipelineState* pipelineState = nullptr;
		HRESULT res = device->CreateGraphicsPipelineState(static_cast<const D3D12_GRAPHICS_PIPELINE_STATE_DESC*>(_desc), IID_PPV_ARGS(&pipelineState));
		*_pipelineState = static_cast<SD3D12NativePipelineStateType*>(pipelineState);
		return res;
	}

	uint32_t D3D12CreateComputePipelineState_D3D12Impl(const void* _desc, void* _device, void** _pipelineState) noexcept
	{
		SD3D12NativeDeviceType* device = static_cast<SD3D12NativeDeviceType*>(_device);
		ID3D12PipelineState* pipelineState = nullptr;
		HRESULT res = device->CreateComputePipelineState(static_cast<const D3D12_COMPUTE_PIPELINE_STATE_DESC*>(_desc), IID_PPV_ARGS(&pipelineState));
		*_pipelineState = static_cast<SD3D12NativePipelineStateType*>(pipelineState);
		return res;
	}

	void D3D12CopyDescriptorsSimple_D3D12Impl(void* _device, uint32_t _numDescriptors, size_t _destDescriptorRangeStart, size_t _srcDescriptorRangeStart, int32_t _descriptorType) noexcept
	{
		SD3D12NativeDeviceType* device = static_cast<SD3D12NativeDeviceType*>(_device);

		D3D12_CPU_DESCRIPTOR_HANDLE destDescriptorRangeStart{ _destDescriptorRangeStart };
		D3D12_CPU_DESCRIPTOR_HANDLE srcDescriptorRangeStart{ _srcDescriptorRangeStart };

		device->CopyDescriptorsSimple(_numDescriptors, destDescriptorRangeStart, srcDescriptorRangeStart, static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(_descriptorType));
	}

	void D3D12GetDescriptorHandleIncrementSize_D3D12Impl(void* _device, int32_t _descriptorType, uint32_t* _incrementSize) noexcept
	{
		SD3D12NativeDeviceType* device = static_cast<SD3D12NativeDeviceType*>(_device);
		*_incrementSize = device->GetDescriptorHandleIncrementSize(static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(_descriptorType));
	}

	uint32_t D3D12ResetCommandAllocator_D3D12Impl(void* _commandAllocator) noexcept
	{
		SD3D12NativeCommandAllocatorType* commandAllocator = static_cast<SD3D12NativeCommandAllocatorType*>(_commandAllocator);

		HRESULT res = commandAllocator->Reset();
		return res;
	}

	uint32_t D3D12ResetCommandList_D3D12Impl(void* _commandList, void* _commandAllocator) noexcept
	{
		SD3D12NativeCommandListType* commandList = static_cast<SD3D12NativeCommandListType*>(_commandList);
		SD3D12NativeCommandAllocatorType* commandAllocator = static_cast<SD3D12NativeCommandAllocatorType*>(_commandAllocator);

		HRESULT res = commandList->Reset(commandAllocator, nullptr);
		return res;
	}

	uint32_t D3D12CloseCommandList_D3D12Impl(void* _commandList) noexcept
	{
		SD3D12NativeCommandListType* commandList = static_cast<SD3D12NativeCommandListType*>(_commandList);

		HRESULT res = commandList->Close();
		return res;
	}

	void D3D12SetDescriptorHeaps(void* _commandList, uint32_t _descriptorHeapCount, void* const* _descriptorHeaps) noexcept
	{
		SD3D12NativeCommandListType* commandList = static_cast<SD3D12NativeCommandListType*>(_commandList);

		ID3D12DescriptorHeap* descriptorHeap[16];

		for (uint32_t i = 0; i != _descriptorHeapCount; ++i)
			descriptorHeap[i] = static_cast<SD3D12NativeDescriptorHeapType*>(_descriptorHeaps[i]);

		commandList->SetDescriptorHeaps(_descriptorHeapCount, descriptorHeap);
	}

	void D3D12DrawIndexedInstanced_D3D12Impl(void* _commandList, uint32_t _indexCountPerInstance, uint32_t _instanceCount, uint32_t _startIndexLocation, int32_t _baseVertexLocation, uint32_t _startInstanceLocation) noexcept
	{
		SD3D12NativeCommandListType* commandList = static_cast<SD3D12NativeCommandListType*>(_commandList);

		commandList->DrawIndexedInstanced(_indexCountPerInstance, _instanceCount, _startIndexLocation, _baseVertexLocation, _startInstanceLocation);
	}

	void D3D12Dispatch_D3D12Impl(void* _commandList, uint32_t _threadGroupCountX, uint32_t _threadGroupCountY, uint32_t _threadGroupCountZ) noexcept
	{
		SD3D12NativeCommandListType* commandList = static_cast<SD3D12NativeCommandListType*>(_commandList);

		commandList->Dispatch(_threadGroupCountX, _threadGroupCountY, _threadGroupCountZ);
	}

	void D3D12CopyBufferRegion_D3D12Impl(void* _commandList, void* _destBuffer, uint64_t _destOffset, void* _srcBuffer, uint64_t _srcOffset, uint64_t _numBytes) noexcept
	{
		SD3D12NativeCommandListType* commandList = static_cast<SD3D12NativeCommandListType*>(_commandList);

		SD3D12NativeResourceType* destBuffer = static_cast<SD3D12NativeResourceType*>(_destBuffer);
		SD3D12NativeResourceType* srcBuffer = static_cast<SD3D12NativeResourceType*>(_srcBuffer);

		commandList->CopyBufferRegion(destBuffer, _destOffset, srcBuffer, _srcOffset, _numBytes);
	}

	void D3D12CopyResource_D3D12Impl(void* _commandList, void* _destResource, void* _srcResource) noexcept
	{
		SD3D12NativeCommandListType* commandList = static_cast<SD3D12NativeCommandListType*>(_commandList);

		SD3D12NativeResourceType* destResource = static_cast<SD3D12NativeResourceType*>(_destResource);
		SD3D12NativeResourceType* srcResource = static_cast<SD3D12NativeResourceType*>(_srcResource);

		commandList->CopyResource(destResource, srcResource);
	}

	void D3D12ResourceBarrier_D3D12Impl(void* _commandList, uint32_t _numBarriers, const void* _barriers) noexcept
	{
		SD3D12NativeCommandListType* commandList = static_cast<SD3D12NativeCommandListType*>(_commandList);

		commandList->ResourceBarrier(_numBarriers, static_cast<const D3D12_RESOURCE_BARRIER*>(_barriers));
	}

	void D3D12IASetPrimitiveTopology_D3D12Impl(void* _commandList, uint32_t _primitiveTopology) noexcept
	{
		SD3D12NativeCommandListType* commandList = static_cast<SD3D12NativeCommandListType*>(_commandList);

		commandList->IASetPrimitiveTopology(static_cast<D3D12_PRIMITIVE_TOPOLOGY>(_primitiveTopology));
	}

	void D3D12IASetIndexBuffer_D3D12Impl(void* _commandList, const void* _indexBufferView) noexcept
	{
		SD3D12NativeCommandListType* commandList = static_cast<SD3D12NativeCommandListType*>(_commandList);

		commandList->IASetIndexBuffer(static_cast<const D3D12_INDEX_BUFFER_VIEW*>(_indexBufferView));
	}

	void D3D12IASetVertexBuffers_D3D12Impl(void* _commandList, uint32_t _startSlot, uint32_t _numViews, const void* _views) noexcept
	{
		SD3D12NativeCommandListType* commandList = static_cast<SD3D12NativeCommandListType*>(_commandList);

		commandList->IASetVertexBuffers(_startSlot, _numViews, static_cast<const D3D12_VERTEX_BUFFER_VIEW*>(_views));
	}

	void D3D12RSSetViewports_D3D12Impl(void* _commandList, uint32_t _numViewports, const void* _viewports) noexcept
	{
		SD3D12NativeCommandListType* commandList = static_cast<SD3D12NativeCommandListType*>(_commandList);

		commandList->RSSetViewports(_numViewports, static_cast<const D3D12_VIEWPORT*>(_viewports));
	}

	void D3D12RSSetScissorRects_D3D12Impl(void* _commandList, uint32_t _numRects, const void* _rects) noexcept
	{
		SD3D12NativeCommandListType* commandList = static_cast<SD3D12NativeCommandListType*>(_commandList);

		commandList->RSSetScissorRects(_numRects, static_cast<const D3D12_RECT*>(_rects));
	}

	void D3D12OMSetRenderTargets_D3D12Impl(void* _commandList, uint32_t _numRenderTargetDescriptors, const void* _renderTargetDescriptors, bool _rtsSingleHandleToDescriptorRange, const void* _depthStencilDescriptor) noexcept
	{
		SD3D12NativeCommandListType* commandList = static_cast<SD3D12NativeCommandListType*>(_commandList);

		commandList->OMSetRenderTargets(_numRenderTargetDescriptors, static_cast<const D3D12_CPU_DESCRIPTOR_HANDLE*>(_renderTargetDescriptors), _rtsSingleHandleToDescriptorRange, static_cast<const D3D12_CPU_DESCRIPTOR_HANDLE*>(_depthStencilDescriptor));
	}

	void D3D12OMSetBlendFactor_D3D12Impl(void* _commandList, const float* _blendFactor) noexcept
	{
		SD3D12NativeCommandListType* commandList = static_cast<SD3D12NativeCommandListType*>(_commandList);

		commandList->OMSetBlendFactor(_blendFactor);
	}

	void D3D12OMSetStencilRef_D3D12Impl(void* _commandList, uint32_t _stencilRef) noexcept
	{
		SD3D12NativeCommandListType* commandList = static_cast<SD3D12NativeCommandListType*>(_commandList);

		commandList->OMSetStencilRef(_stencilRef);
	}

	void D3D12SetPipelineState_D3D12Impl(void* _commandList, void* _pipelineState) noexcept
	{
		SD3D12NativeCommandListType* commandList = static_cast<SD3D12NativeCommandListType*>(_commandList);

		commandList->SetPipelineState(static_cast<SD3D12NativePipelineStateType*>(_pipelineState));
	}

	void D3D12SetGraphicsRootSignature_D3D12Impl(void* _commandList, void* _rootSignature) noexcept
	{
		SD3D12NativeCommandListType* commandList = static_cast<SD3D12NativeCommandListType*>(_commandList);

		commandList->SetGraphicsRootSignature(static_cast<SD3D12NativeRootSignatureType*>(_rootSignature));
	}

	void D3D12SetComputeRootSignature_D3D12Impl(void* _commandList, void* _rootSignature) noexcept
	{
		SD3D12NativeCommandListType* commandList = static_cast<SD3D12NativeCommandListType*>(_commandList);

		commandList->SetComputeRootSignature(static_cast<SD3D12NativeRootSignatureType*>(_rootSignature));
	}

	void D3D12SetGraphicsRootConstantBufferView_D3D12Impl(void* _commandList, uint32_t _rootParameterIndex, uint64_t _bufferLocation) noexcept
	{
		SD3D12NativeCommandListType* commandList = static_cast<SD3D12NativeCommandListType*>(_commandList);

		commandList->SetGraphicsRootConstantBufferView(_rootParameterIndex, _bufferLocation);
	}

	void D3D12SetComputeRootConstantBufferView_D3D12Impl(void* _commandList, uint32_t _rootParameterIndex, uint64_t _bufferLocation) noexcept
	{
		SD3D12NativeCommandListType* commandList = static_cast<SD3D12NativeCommandListType*>(_commandList);

		commandList->SetComputeRootConstantBufferView(_rootParameterIndex, _bufferLocation);
	}

	void D3D12SetGraphicsRootDescriptorTable_D3D12Impl(void* _commandList, uint32_t _rootParameterIndex, uint64_t _baseDescriptor) noexcept
	{
		SD3D12NativeCommandListType* commandList = static_cast<SD3D12NativeCommandListType*>(_commandList);

		D3D12_GPU_DESCRIPTOR_HANDLE baseDescriptor;
		baseDescriptor.ptr = _baseDescriptor;

		commandList->SetGraphicsRootDescriptorTable(_rootParameterIndex, baseDescriptor);
	}

	void D3D12SetComputeRootDescriptorTable_D3D12Impl(void* _commandList, uint32_t _rootParameterIndex, uint64_t _baseDescriptor) noexcept
	{
		SD3D12NativeCommandListType* commandList = static_cast<SD3D12NativeCommandListType*>(_commandList);

		D3D12_GPU_DESCRIPTOR_HANDLE baseDescriptor;
		baseDescriptor.ptr = _baseDescriptor;

		commandList->SetComputeRootDescriptorTable(_rootParameterIndex, baseDescriptor);
	}

	void D3D12ClearDepthStencilView_D3D12Impl(void* _commandList, size_t _depthStencilView, int32_t _clearFlags, float _depth, uint8_t _stencil, uint32_t _numRects, const void* _rects) noexcept
	{
		SD3D12NativeCommandListType* commandList = static_cast<SD3D12NativeCommandListType*>(_commandList);

		D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView;
		depthStencilView.ptr = _depthStencilView;

		commandList->ClearDepthStencilView(depthStencilView, static_cast<D3D12_CLEAR_FLAGS>(_clearFlags), _depth, _stencil, _numRects, static_cast<const D3D12_RECT*>(_rects));
	}

	void D3D12ClearRenderTargetView_D3D12Impl(void* _commandList, size_t _renderTargetView, const float* _colorRGBA, uint32_t _numRects, const void* _rects) noexcept
	{
		SD3D12NativeCommandListType* commandList = static_cast<SD3D12NativeCommandListType*>(_commandList);

		D3D12_CPU_DESCRIPTOR_HANDLE renderTargetView;
		renderTargetView.ptr = _renderTargetView;

		commandList->ClearRenderTargetView(renderTargetView, _colorRGBA, _numRects, static_cast<const D3D12_RECT*>(_rects));
	}

	void D3D12ClearUnorderedAccessViewUint_D3D12Impl(void* _commandList, uint64_t _shaderVisibleViewGpuHandle, size_t _cpuHandle, void* _resource, const uint32_t* _value, uint32_t _numRect, const void* _rects) noexcept
	{
		SD3D12NativeCommandListType* commandList = static_cast<SD3D12NativeCommandListType*>(_commandList);

		D3D12_GPU_DESCRIPTOR_HANDLE shaderVisibleViewGpuHandle;
		shaderVisibleViewGpuHandle.ptr = _shaderVisibleViewGpuHandle;

		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
		cpuHandle.ptr = _cpuHandle;

		commandList->ClearUnorderedAccessViewUint(shaderVisibleViewGpuHandle, cpuHandle, static_cast<SD3D12NativeResourceType*>(_resource), _value, _numRect, static_cast<const D3D12_RECT*>(_rects));
	}

	void D3D12ClearUnorderedAccessViewFloat_D3D12Impl(void* _commandList, uint64_t _shaderVisibleViewGpuHandle, size_t _cpuHandle, void* _resource, const float* _value, uint32_t _numRect, const void* _rects) noexcept
	{
		SD3D12NativeCommandListType* commandList = static_cast<SD3D12NativeCommandListType*>(_commandList);

		D3D12_GPU_DESCRIPTOR_HANDLE shaderVisibleViewGpuHandle;
		shaderVisibleViewGpuHandle.ptr = _shaderVisibleViewGpuHandle;

		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
		cpuHandle.ptr = _cpuHandle;

		commandList->ClearUnorderedAccessViewFloat(shaderVisibleViewGpuHandle, cpuHandle, static_cast<SD3D12NativeResourceType*>(_resource), _value, _numRect, static_cast<const D3D12_RECT*>(_rects));
	}

	void D3D12ExecuteCommandLists_D3D12Impl(void* _commandQueue, uint32_t _commandListCount, void* const* _commandLists) noexcept
	{
		SD3D12NativeCommandQueueType* commandQueue = static_cast<SD3D12NativeCommandQueueType*>(_commandQueue);

		ID3D12CommandList* commandList[16];
		for (uint32_t i = 0; i != _commandListCount; ++i)
			commandList[i] = static_cast<SD3D12NativeCommandListType*>(_commandLists[i]);

		commandQueue->ExecuteCommandLists(_commandListCount, commandList);
	}

	uint32_t D3D12Signal_D3D12Impl(void* _commandQueue, void* _fence, uint64_t _value) noexcept
	{
		SD3D12NativeCommandQueueType* commandQueue = static_cast<SD3D12NativeCommandQueueType*>(_commandQueue);
		SD3D12NativeFenceType* fence = static_cast<SD3D12NativeFenceType*>(_fence);

		HRESULT res = commandQueue->Signal(fence, _value);
		return res;
	}

	uint64_t D3D12GetCompletedValue_D3D12Impl(void* _fence) noexcept
	{
		SD3D12NativeFenceType* fence = static_cast<SD3D12NativeFenceType*>(_fence);

		return fence->GetCompletedValue();
	}

	uint32_t D3D12WaitUntilFenceComplete_D3D12Impl(void* _fence, uint64_t _fenceValue) noexcept
	{
		SD3D12NativeFenceType* fence = static_cast<SD3D12NativeFenceType*>(_fence);

		HRESULT res = S_OK;

		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		if (event != nullptr)
		{
			res = fence->SetEventOnCompletion(_fenceValue, event);
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}
		else
		{
			res = E_FAIL;
		}

		return res;
	}

	void D3D12GetGPUVirtualAddress_D3D12Impl(void* _resource, uint64_t* _gpuVirtualAddress) noexcept
	{
		SD3D12NativeResourceType* resource = static_cast<SD3D12NativeResourceType*>(_resource);
		*_gpuVirtualAddress = resource->GetGPUVirtualAddress();
	}

	void D3D12GetCPUDescriptorHandleForHeapStart_D3D12Impl(void* _descriptorHeap, size_t* _startCpuDescriptorHandle) noexcept
	{
		SD3D12NativeDescriptorHeapType* heap = static_cast<SD3D12NativeDescriptorHeapType*>(_descriptorHeap);

		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = heap->GetCPUDescriptorHandleForHeapStart();
		*_startCpuDescriptorHandle = cpuHandle.ptr;
	}

	void D3D12GetGPUDescriptorHandleForHeapStart_D3D12Impl(void* _descriptorHeap, uint64_t* _startGpuDescriptorHandle) noexcept
	{
		SD3D12NativeDescriptorHeapType* heap = static_cast<SD3D12NativeDescriptorHeapType*>(_descriptorHeap);

		D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = heap->GetGPUDescriptorHandleForHeapStart();
		*_startGpuDescriptorHandle = gpuHandle.ptr;
	}

	uint32_t D3D12SerializeRootSignature_D3D12Impl(const void* _desc, int32_t _verision, std::vector<uint8_t>& _serlizedBlob, std::vector<uint8_t> _errorBlob) noexcept
	{
		_serlizedBlob.clear();
		_errorBlob.clear();

		ID3DBlob* serlizedBlob = nullptr;
		ID3DBlob* errorBlob = nullptr;

		HRESULT res = D3D12SerializeRootSignature(static_cast<const D3D12_ROOT_SIGNATURE_DESC*>(_desc), static_cast<D3D_ROOT_SIGNATURE_VERSION>(_verision), &serlizedBlob, &errorBlob);

		if (serlizedBlob)
		{
			_serlizedBlob.resize(serlizedBlob->GetBufferSize());
			memcpy_s(&_serlizedBlob[0], _serlizedBlob.size(), serlizedBlob->GetBufferPointer(), serlizedBlob->GetBufferSize());
			serlizedBlob->Release();
		}

		if (errorBlob)
		{
			_errorBlob.resize(errorBlob->GetBufferSize());
			memcpy_s(&_errorBlob[0], _errorBlob.size(), errorBlob->GetBufferPointer(), errorBlob->GetBufferSize());
			errorBlob->Release();
		}

		return res;
	}

	uint32_t D3D12GetPipelineStateCachedBlob(void* _pipelineState, std::vector<uint8_t>& _cachedBlob) noexcept
	{
		SD3D12NativePipelineStateType* pipelineState = static_cast<SD3D12NativePipelineStateType*>(_pipelineState);

		ID3DBlob* blob;
		HRESULT res = pipelineState->GetCachedBlob(&blob);

		if (blob)
		{
			_cachedBlob.resize(blob->GetBufferSize());
			memcpy_s(&_cachedBlob[0], _cachedBlob.size(), blob->GetBufferPointer(), blob->GetBufferSize());
			blob->Release();
		}

		return res;
	}

	uint32_t D3D12SwapChainGetBuffer_D3D12Impl(void* _swapChain, uint32_t _index, void** _renderTaget) noexcept
	{
		SD3D12NativeSwapChainType* swapChain = static_cast<SD3D12NativeSwapChainType*>(_swapChain);

		ID3D12Resource* renderTarget = nullptr;
		HRESULT res = swapChain->GetBuffer(_index, IID_PPV_ARGS(&renderTarget));

		*_renderTaget = renderTarget;
		return res;
	}
}
