#include "GMQuantCore/Util/QuantMisc.h"

std::string FormatTime_yy_mm_dd_hh_mm_ss(std::chrono::system_clock::time_point _time) noexcept
{
	_time += std::chrono::hours(8);
	std::chrono::year_month_day ymd = std::chrono::floor<std::chrono::days>(_time);
	std::chrono::hh_mm_ss<std::chrono::seconds> hms(std::chrono::floor<std::chrono::seconds>(_time) - std::chrono::floor<std::chrono::days>(_time));

	return std::format("{:%F} {:%T}", ymd, hms);
}

std::string FormatTime_yy_mm_dd(std::chrono::year_month_day _time) noexcept
{
	return std::format("{:%F}", _time);
}

std::string FormatTime_hh_mm_ss(std::chrono::hh_mm_ss<std::chrono::seconds> _time) noexcept
{
	return std::format("{:%T}", _time);
}

std::chrono::year_month_day ConvertToYMD(std::chrono::system_clock::time_point _time) noexcept
{
	_time += std::chrono::hours(8);
	return std::chrono::floor<std::chrono::days>(_time);
}

std::chrono::hh_mm_ss<std::chrono::seconds> ConvertToHMS(std::chrono::system_clock::time_point _time) noexcept
{
	return std::chrono::hh_mm_ss(std::chrono::floor<std::chrono::seconds>(_time) - std::chrono::floor<std::chrono::days>(_time));
}
