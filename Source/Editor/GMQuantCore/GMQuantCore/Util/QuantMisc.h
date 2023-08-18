#pragma once

#include <chrono>
#include <string>

std::string FormatTime_yy_mm_dd_hh_mm_ss(std::chrono::system_clock::time_point _time) noexcept;

std::string FormatTime_yy_mm_dd(std::chrono::year_month_day _time) noexcept;
std::string FormatTime_hh_mm_ss(std::chrono::hh_mm_ss<std::chrono::seconds> _time) noexcept;

std::chrono::year_month_day ConvertToYMD(std::chrono::system_clock::time_point _time) noexcept; 
std::chrono::hh_mm_ss<std::chrono::seconds> ConvertToHMS(std::chrono::system_clock::time_point _time) noexcept;
