#pragma once

#include "GMQuantCore/QuantStrategyContext.h"

#include <chrono>
#include "strategy.h"

struct SBacktestConfig;
class SQuantStrategyBase;

TODO("需要针对当前时间对接口做相应的封装")
class SGMQuantStrategyContextImpl : public Strategy, public IQuantStrategyContext
{
public:
	// IQuantStrategyContext Interface
	std::chrono::system_clock::time_point GetEarliestBacktestTime() noexcept override;
	std::chrono::system_clock::time_point GetLatestBacktestTime() noexcept override;
	std::chrono::system_clock::time_point GetNowTime() noexcept override;

	std::vector<std::string> GetAllStockSymbols() noexcept override;

	std::vector<STick> GetHistoryTick(const std::string& _symbol, std::chrono::system_clock::time_point _startTime, std::chrono::system_clock::time_point _endTime, EAdjustMode _adjustMode = EAdjustMode::ADJUST_BACKWARD) noexcept override;
	std::vector<STick> GetHistoryNTick(const std::string& _symbol, uint64_t _n, std::chrono::system_clock::time_point _endTime, EAdjustMode _adjustMode = EAdjustMode::ADJUST_BACKWARD) noexcept override;
	std::vector<STick> GetLastestNTick(const std::string& _symbol, uint64_t _n, EAdjustMode _adjustMode = EAdjustMode::ADJUST_BACKWARD);

	std::vector<SBar> GetHistoryBar(const std::string& _symbol, std::chrono::system_clock::time_point _startTime, std::chrono::system_clock::time_point _endTime, EFrequency _frequency, EAdjustMode _adjustMode) noexcept;
	std::vector<SBar> GetHistoryNBar(const std::string& _symbol, uint64_t _n, std::chrono::system_clock::time_point _endTime, EFrequency _frequency, EAdjustMode _adjustMode) noexcept;
	std::vector<SBar> GetLastestNBar(const std::string& _symbol, uint64_t _n, EFrequency _frequency, EAdjustMode _adjustMode) noexcept;

	void BuyVolume(const std::string& _symbol, uint64_t _volume, double _price = 0) noexcept override;
	void SellVolume(const std::string& _symbol, uint64_t _volume, double _price = 0) noexcept override;

	SCash GetCash() noexcept override;
	SPosition GetPosition(const std::string& _symbol) noexcept override;
	std::vector<SPosition> GetPositionList()noexcept override;
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
	std::string FormatFrequency(EFrequency _frequency) const noexcept;

	std::vector<STick> ConvertToTickList(DataArray<Tick>* _rawTickList) noexcept;
	std::vector<SBar> ConvertToBarList(DataArray<Bar>* _rawBarList) noexcept;
	std::vector<SPosition> ConvertToPositionList(DataArray<Position>* _rawPositionList) noexcept;

	SQuantStrategyBase* mCurrentStrategy;
	bool mIsBacktestMode = true;
};
