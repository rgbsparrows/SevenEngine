#pragma once

#include "Core/Util/Assert.h"
#include "Core/Util/NameOf.h"
#include "Core/Util/Algorithm.h"
#include "Core/Util/TemplateUtil.h"

#include <thread>
#include <functional>
#include "Thread.h"

using STaskIdentify = THandleType<CalcStrHash(L"STaskHandle"), uint64_t>;

struct STask
{
#if WITH_DEBUG_CODE
	std::string mTaskDebugName = "UnnamedTask";
#endif
	std::function<void(size_t)> mTaskFunc;
	std::vector<STaskIdentify> mDependencyTaskList;
	std::atomic<bool> mBeginExecuate = false;
	std::atomic<bool> mIsExecuated = true;

	// 不生成实质性的拷贝函数
	STask() noexcept = default;
	STask(const STask&) noexcept : STask() {}
};

class STaskGraph
{
public:
	STaskGraph() noexcept{}
	~STaskGraph() noexcept;

	void Construct(size_t _threadCount, size_t _taskCapacity = 128, std::thread::id _mainThreadId = std::this_thread::get_id()) noexcept;

	STaskIdentify AddTask(const std::string& _taskName, std::function<void(size_t)>&& _taskFunc, const std::vector<STaskIdentify>& _depdencyTask) noexcept;
	bool IsTaskExecuted(STaskIdentify _taskHandle) noexcept;
	bool IsTaskExecuted(const std::vector<STaskIdentify>& _taskHandleList) noexcept;
	void WaitTask(STaskIdentify _taskHandle) noexcept;
	void WaitTask(const std::vector<STaskIdentify>& _taskHandleList) noexcept;

	size_t GetSubThreadCount() const noexcept { return mThreadList.size(); }
	size_t GetTaskCount() const noexcept { return mTaskCount; }

private:
	void SubThreadMain(size_t _threadIndex) noexcept;

private:
	std::thread::id mMainThreadID;

	std::vector<std::thread> mThreadList;
	std::vector<STask> mTaskList;

	std::atomic<uint64_t> mExecuatedTaskEnd = 0;
	std::atomic<uint64_t> mTaskCount = 0;

	std::atomic<bool> mRequireExit = false;
};
