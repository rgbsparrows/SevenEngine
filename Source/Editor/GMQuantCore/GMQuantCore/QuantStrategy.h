#pragma once

#include "QuantType.h"
#include "Core/Misc/Thread.h"

#include <mutex>
#include <vector>
#include <atomic>
#include <string>

__interface IQuantStrategyContext;

enum class EStrategyExecuteState
{
	Unexecuted,
	WaitingForExecute,
	Executing,
	Executed,
};

class SQuantStrategyBase
{
public:
	EStrategyExecuteState GetStrategyExecuteState() const noexcept { return mExecuteState; }
	void SetStrategyExecuteState(EStrategyExecuteState _state) noexcept { mExecuteState = _state; }

	bool IsRequireExit() const noexcept { return mRequireExit; }
	void RequireExit() noexcept { mRequireExit = true; }

	std::string GetDisplayName() const noexcept { return mDisplayName; }
	void SetDisplayName(const std::string& _displayName) noexcept { mDisplayName = _displayName; }

	void Init(IQuantStrategyContext* _context) noexcept
	{
		std::lock_guard lock(mMutex);
		OnInit(_context);
	}

	void Tick(IQuantStrategyContext* _context) noexcept
	{
		std::lock_guard lock(mMutex);
		OnTick(_context);
	}

	void Schedule(IQuantStrategyContext* _context) noexcept
	{
		std::lock_guard lock(mMutex);
		OnSchedule(_context);
	}

	void Stop(IQuantStrategyContext* _context) noexcept
	{
		std::lock_guard lock(mMutex);
		OnStop(_context);
	}

	void Reset() noexcept
	{
		RequireExit();
		Thread::YieldUntilValue(mExecuteState, EStrategyExecuteState::Executed);

		mExecuteState = EStrategyExecuteState::Unexecuted;
		mRequireExit = false;

		std::lock_guard lock(mMutex);
		OnReset();
	}

	void Gui() noexcept
	{
		std::lock_guard lock(mMutex);
		OnGui();
	}

	void ParameterGui() noexcept
	{
		std::lock_guard lock(mMutex);
		OnParameterGui();
	}

	void StatsGui() noexcept
	{
		std::lock_guard lock(mMutex);
		OnStatsGui();
	}

public:
	virtual std::vector<std::chrono::hh_mm_ss<std::chrono::seconds>> GetScheduleTimeList() const noexcept { return {}; }

	virtual void OnInit(IQuantStrategyContext* _context) noexcept {}
	virtual void OnTick(IQuantStrategyContext* _context) noexcept {} // 仅交易模式可用
	virtual void OnSchedule(IQuantStrategyContext* _context) noexcept {}
	virtual void OnStop(IQuantStrategyContext* _context) noexcept {}

	virtual void OnReset() noexcept {}
	virtual void OnGui() noexcept {}
	virtual void OnParameterGui() noexcept {}
	virtual void OnStatsGui() noexcept {}

private:
	std::string mDisplayName;

	std::atomic<EStrategyExecuteState> mExecuteState = EStrategyExecuteState::Unexecuted;
	std::atomic_bool mRequireExit = false;
	mutable std::mutex mMutex;
};
