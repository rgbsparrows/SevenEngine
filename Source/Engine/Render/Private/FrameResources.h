#pragma once

struct SFrameResource
{
	bool mRequireExit = false;

	HANDLE mGameThreadFrameResourceReadyEvent = nullptr;
	HANDLE mRenderThreadFrameResourceReadyEvent = nullptr;
	uint64_t mGpuFence = 0;
};
