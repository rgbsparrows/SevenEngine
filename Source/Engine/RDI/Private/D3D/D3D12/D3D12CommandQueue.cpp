#include "D3D/D3DUtil.h"
#include "Core/Macros/Assert.h"
#include "D3D/D3D12/D3D12CommandList.h"
#include "D3D/D3D12/D3D12CommandQueue.h"
#include "D3D/D3D12/Warper/D3D12ImplWarper.h"

#include <thread>

void SD3D12CommandQueue::Init(void* _commandQueueNativePtr, void* _fenceNativePtr) noexcept
{
	mCommandQueueNativePtr = _commandQueueNativePtr;
	mFenceNativePtr = _fenceNativePtr;
	mCurrentFenceValue = 0;
}

void SD3D12CommandQueue::ExecuteCommandLists(uint32_t _commandListCount, IRDICommandList* const* _commandLists) noexcept
{
	CHECK(_commandListCount <= 16);
	
	void* commandList[16];
	for (uint32_t i = 0; i != _commandListCount; ++i)
		commandList[i] = static_cast<SD3D12CommandList*>(_commandLists[i])->GetCommandListNativePtr();
	
	D3D12APIWarp_Impl::D3D12ExecuteCommandLists_D3D12Impl(GetCommandQueueNativePtr(), _commandListCount, commandList);
}

uint64_t SD3D12CommandQueue::Signal() noexcept
{
	VERIFY_D3D_RETURN(D3D12APIWarp_Impl::D3D12Signal_D3D12Impl(GetCommandQueueNativePtr(), GetFenceNativePtr(), ++mCurrentFenceValue));
	return mCurrentFenceValue;
}

uint64_t SD3D12CommandQueue::GetCompletedValue() noexcept
{
	return D3D12APIWarp_Impl::D3D12GetCompletedValue_D3D12Impl(GetFenceNativePtr());
}

void SD3D12CommandQueue::YieldUntilCompletion(uint64_t _fenceValue) noexcept
{
	while (GetCompletedValue() < _fenceValue)
		std::this_thread::yield();
}

void SD3D12CommandQueue::WaitForCompletion(uint64_t _fenceValue) noexcept
{
	VERIFY_D3D_RETURN(D3D12APIWarp_Impl::D3D12WaitUntilFenceComplete_D3D12Impl(GetFenceNativePtr(), _fenceValue));
}
