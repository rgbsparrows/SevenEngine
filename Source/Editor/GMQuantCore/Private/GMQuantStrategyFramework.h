#pragma once

#include "GMQuantCore/QuantStrategyContext.h"

#include <chrono>
#include "strategy.h"

struct SBacktestConfig;
class SQuantStrategyBase;

class SGMQuantStrategyContextImpl : public Strategy, public IQuantStrategyContext
{
public:
	// IQuantStrategyContext Interface
	std::chrono::system_clock::time_point GetEarliestBacktestTime() const noexcept override;
	std::chrono::system_clock::time_point GetLatestBacktestTime() const noexcept override;
	std::chrono::system_clock::time_point GetNowTime() noexcept;
	// End IQuantStrategyContext

public:
	void SetUserToken(const std::string& _userToken) noexcept;
	void ExecuteBacktestStrategy(SQuantStrategyBase* _strategy, const std::string& _strategyId, const SBacktestConfig& _backtestConfig) noexcept;
	void ExecuteRealtimeStrategy(SQuantStrategyBase* _strategy, const std::string& _strategyId) noexcept;

public:
	// Strategy Interface
	void on_init() override;
	void on_tick(Tick*) override;
	void on_schedule(const char*, const char*) override;
	void on_backtest_finished(Indicator*) override;
	void on_stop() override;
	// End Strategy

private:
	std::string FormatTime_yy_mm_dd_hh_mm_ss(std::chrono::system_clock::time_point _time) const noexcept;
	std::string FormatTime_yy_mm_dd(std::chrono::year_month_day _time) const noexcept;
	std::string FormatTime_hh_mm_ss(std::chrono::hh_mm_ss<std::chrono::seconds> _time) const noexcept;

	SQuantStrategyBase* mCurrentStrategy;
	bool mIsBacktestMode = true;
};
