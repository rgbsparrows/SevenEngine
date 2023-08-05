#include "Core/Misc/TaskGraph.h"

STaskGraph::~STaskGraph() noexcept
{
	mRequireExit = true;
	for (std::thread& thread : mThreadList)
		thread.join();

	// Main thread execute remain task
	for (uint64_t i = mExecuatedTaskEnd; i != mTaskCount; ++i)
	{
		STask& task = mTaskList[i % mTaskList.size()];
		if (task.mIsExecuated == false)
		{
			task.mTaskFunc(0);
			task.mIsExecuated = true;
		}
	}
}

void STaskGraph::Construct(size_t _threadCount, size_t _taskCapacity, std::thread::id _mainThreadId) noexcept
{
	mMainThreadID = _mainThreadId;

	mThreadList.reserve(_threadCount);
	for (size_t i = 0; i < _threadCount; ++i)
		mThreadList.emplace_back(std::thread(std::mem_fn(&STaskGraph::SubThreadMain), this, i));

	for (size_t i = 0; i != _taskCapacity; ++i)
		mTaskList.push_back(STask());
}

STaskIdentify STaskGraph::AddTask(const std::string& _taskName, std::function<void(size_t)>&& _taskFunc, const std::vector<STaskIdentify>& _depdencyTask) noexcept
{
	CHECK(std::this_thread::get_id() == mMainThreadID);

	STask& needFillTask = mTaskList[mTaskCount % mTaskList.size()];
	Thread::YieldUntilValue(needFillTask.mIsExecuated, true);

	for (uint64_t i = mExecuatedTaskEnd; i != mTaskCount; ++i)
	{
		if (mTaskList[i % mTaskList.size()].mIsExecuated == false)
			break;

		++mExecuatedTaskEnd;
	}

#if WITH_DEBUG_CODE
	needFillTask.mTaskDebugName = _taskName;
#endif
	needFillTask.mTaskFunc = std::move(_taskFunc);
	needFillTask.mDependencyTaskList = _depdencyTask;
	Sort(needFillTask.mDependencyTaskList, [](STaskIdentify _left, STaskIdentify _right) { return _right < _left; });
	needFillTask.mBeginExecuate = false;
	needFillTask.mIsExecuated = false;

	STaskIdentify taskHandle(mTaskCount++);
	return taskHandle;
}

bool STaskGraph::IsTaskExecuted(STaskIdentify _taskHandle) noexcept
{
	if (static_cast<uint64_t>(_taskHandle) < mExecuatedTaskEnd)
		return true;

	return mTaskList[static_cast<uint64_t>(_taskHandle) % mTaskList.size()].mIsExecuated;
}

bool STaskGraph::IsTaskExecuted(const std::vector<STaskIdentify>& _taskHandleList) noexcept
{
	for (STaskIdentify taskHandle : _taskHandleList)
		if (IsTaskExecuted(taskHandle) == false)
			return false;

	return true;
}

void STaskGraph::WaitTask(STaskIdentify _taskHandle) noexcept
{
	Thread::YieldUntil([&]() { return IsTaskExecuted(_taskHandle); });
}

void STaskGraph::WaitTask(const std::vector<STaskIdentify>& _taskHandleList) noexcept
{
	Thread::YieldUntil([&]() { return IsTaskExecuted(_taskHandleList); });
}

void STaskGraph::SubThreadMain(size_t _threadIndex) noexcept
{
	while (mRequireExit == false)
	{
		for (uint64_t i = mExecuatedTaskEnd; i != mTaskCount; ++i)
		{
			STask& task = mTaskList[i % mTaskList.size()];
			if (task.mBeginExecuate == false && IsTaskExecuted(task.mDependencyTaskList))
			{
				bool beginExecute = false;
				if (task.mBeginExecuate.compare_exchange_strong(beginExecute, true))
				{
					task.mTaskFunc(_threadIndex);
					task.mIsExecuated = true;
				}
			}
		}

		std::this_thread::yield();
	}
}
