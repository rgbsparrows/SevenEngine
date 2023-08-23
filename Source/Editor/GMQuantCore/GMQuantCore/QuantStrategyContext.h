#pragma once

#include "QuantType.h"

#include <stdint.h>
#include <chrono>

// 专注股票与ETF,场外基金
__interface IQuantStrategyContext
{
	std::chrono::system_clock::time_point GetEarliestBacktestTime() noexcept;
	std::chrono::system_clock::time_point GetLatestBacktestTime() noexcept;

	std::chrono::system_clock::time_point GetNowTime() noexcept;

	std::vector<std::string> GetAllStockSymbols() noexcept;

	std::vector<STick> GetHistoryTick(const std::string& _symbol, std::chrono::system_clock::time_point _startTime, std::chrono::system_clock::time_point _endTime, EAdjustMode _adjustMode = EAdjustMode::ADJUST_BACKWARD) noexcept;
	std::vector<STick> GetHistoryNTick(const std::string& _symbol, uint64_t _n, std::chrono::system_clock::time_point _endTime, EAdjustMode _adjustMode = EAdjustMode::ADJUST_BACKWARD) noexcept;
	std::vector<STick> GetLastestNTick(const std::string& _symbol, uint64_t _n, EAdjustMode _adjustMode = EAdjustMode::ADJUST_BACKWARD);

	std::vector<SBar> GetHistoryBar(const std::string& _symbol, std::chrono::system_clock::time_point _startTime, std::chrono::system_clock::time_point _endTime, EFrequency _frequency = EFrequency::FREQUENCY_1DAY, EAdjustMode _adjustMode = EAdjustMode::ADJUST_BACKWARD) noexcept;
	std::vector<SBar> GetHistoryNBar(const std::string& _symbol, uint64_t _n, std::chrono::system_clock::time_point _endTime, EFrequency _frequency = EFrequency::FREQUENCY_1DAY, EAdjustMode _adjustMode = EAdjustMode::ADJUST_BACKWARD) noexcept;
	std::vector<SBar> GetLastestNBar(const std::string& _symbol, uint64_t _n, EFrequency _frequency = EFrequency::FREQUENCY_1DAY, EAdjustMode _adjustMode = EAdjustMode::ADJUST_BACKWARD) noexcept;

	void BuyVolume(const std::string& _symbol, uint64_t _volume, double _price = 0) noexcept;
	void SellVolume(const std::string& _symbol, uint64_t _volume, double _price = 0) noexcept;

	SCash GetCash() noexcept;
	SPosition GetPosition(const std::string& _symbol) noexcept;
	std::vector<SPosition> GetPositionList()noexcept;
};
