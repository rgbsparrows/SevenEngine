#pragma once

#include "Core/Util/NameOf.h"
#include "Core/Misc/TaskGraph.h"
#include "Render/RenderModule.h"
#include "Core/Util/TemplateUtil.h"
#include "Core/Container/ElementPool.h"

#include <vector>
#include <functional>

__interface IRDIFactory;
__interface IRDIDevice;
__interface IRDICommandQueue;

__interface IRDICommandList;
__interface IRDICommandAllocator;

using SRenderTaskIdentify = THandleType<CalcStrHash(L"SRenderTaskIdentify"), uint64_t>;

struct SRenderTask
{
	std::string mTaskName;
	std::function<void(IRDICommandList*)> mRenderTask;
	std::vector<SRenderTaskIdentify> mDependencyTaskList;
};

class SRenderContext
{
public:
	void Init(size_t _subRenderThreadCount) noexcept;
	void Clear() noexcept;

	void BeginFrame() noexcept;
	void EndFrame() noexcept;

	IRDIDevice* GetDevice() noexcept { return mDevice; }

	SRenderTaskIdentify AddRenderTask(const std::string& taskName, std::function<void(IRDICommandList*)>&& _renderTask) noexcept;
	SRenderTaskIdentify AddRenderTask(const std::string& taskName, std::function<void(IRDICommandList*)>&& _renderTask, const std::vector<SRenderTaskIdentify>& depdency) noexcept;
	void ExecuteRenderGraph() noexcept;

	void SyncToGpuFrameEnd(bool _force = false) noexcept;

private:
	std::vector<SRenderTaskIdentify> GetAllRenderTask() noexcept;

	IRDICommandList* AllocateCommandList() noexcept;
	void ReleaseCommandList(IRDICommandList* _commandList) noexcept;
	IRDICommandAllocator* GetCommandAllocator(uint32_t _threadIndex = 0) noexcept;

	void ExecuteCommandLists(uint32_t _commandListCount, IRDICommandList* const* _commandLists) noexcept;

	uint64_t GetGpuFence() noexcept;
	void SetGpuFence(uint64_t _fence) noexcept;

private:

	IRDIFactory* mFactory = nullptr;
	IRDIDevice* mDevice = nullptr;
	IRDICommandQueue* mCommandQueue = nullptr;

	std::vector<IRDICommandAllocator*> mCommandAllocatorList[GRenderInfoCount];
	std::vector<IRDICommandList*> mCommandListPool;

	bool mIsSyncToGpuFrameEnd = false;
	uint64_t mGpuFence[GRenderInfoCount] = {0, 0, 0};

	STaskGraph mRenderTaskGraph;
	std::vector<SRenderTask> mRenderTaskList;
};
