#include "D3D/D3DUtil.h"
#include "Core/Util/Assert.h"
#include "Core/Misc/Thread.h"
#include "D3D/D3D12/D3D12CommandList.h"
#include "D3D/D3D12/D3D12CommandQueue.h"
#include "D3D/D3D12/Helper/D3D12Helper.h"

#include <thread>

void SD3D12CommandQueue::Init(ID3D12CommandQueue* _commandQueueNativePtr, ID3D12Fence* _fenceNativePtr) noexcept
{
	mCommandQueueNativePtr = _commandQueueNativePtr;
	mFenceNativePtr = _fenceNativePtr;
	mCurrentFenceValue = 0;
}

void SD3D12CommandQueue::ExecuteCommandLists(uint32_t _commandListCount, IRDICommandList* const* _commandLists) noexcept
{
	CHECK(_commandListCount <= D3D12_EXECUTE_COMMAND_LIST_COUNT);

	ID3D12CommandList* commandList[D3D12_EXECUTE_COMMAND_LIST_COUNT];
	for (uint32_t i = 0; i != _commandListCount; ++i)
		commandList[i] = static_cast<SD3D12CommandList*>(_commandLists[i])->GetNativePtr();

	GetCommandQueueNativePtr()->ExecuteCommandLists(_commandListCount, commandList);
}

uint64_t SD3D12CommandQueue::Signal() noexcept
{
	VERIFY_D3D_RETURN(GetCommandQueueNativePtr()->Signal(GetFenceNativePtr(), ++mCurrentFenceValue));
	return mCurrentFenceValue;
}

uint64_t SD3D12CommandQueue::GetCompletedValue() noexcept
{
	return GetFenceNativePtr()->GetCompletedValue();
}

void SD3D12CommandQueue::YieldUntilCompletion(uint64_t _fenceValue) noexcept
{
	Thread::YieldUntil(
		[&]() { return GetCompletedValue() >= _fenceValue; }
	);
}

void SD3D12CommandQueue::WaitForCompletion(uint64_t _fenceValue) noexcept
{
	if (GetCompletedValue() > _fenceValue)
	{
		HANDLE event = CreateEventW(nullptr, false, false, nullptr);

		CHECK(event != nullptr);

		VERIFY_D3D_RETURN(GetFenceNativePtr()->SetEventOnCompletion(_fenceValue, event));
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}
}
