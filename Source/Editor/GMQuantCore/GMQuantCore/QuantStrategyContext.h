#pragma once

#include <stdint.h>
#include <chrono>

__interface IQuantStrategyContext
{
	std::chrono::system_clock::time_point GetEarliestBacktestTime() const noexcept;
	std::chrono::system_clock::time_point GetLatestBacktestTime() const noexcept;

	std::chrono::system_clock::time_point GetNowTime() noexcept;
};
