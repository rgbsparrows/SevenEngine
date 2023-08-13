#include "GMQuantCore/QuantStrategy.h"

class SMA21Strategy : public SQuantStrategyBase
{
	virtual std::vector<std::chrono::hh_mm_ss<std::chrono::seconds>> GetScheduleTimeList() const noexcept 
	{ 
		return {
			std::chrono::hh_mm_ss<std::chrono::seconds>(std::chrono::hours(20))
		}; 
	}

	virtual void OnInit(IQuantStrategyContext* _context) noexcept {}
	virtual void OnSchedule(IQuantStrategyContext* _context) noexcept {}
	virtual void OnStop(IQuantStrategyContext* _context) noexcept {}

	virtual void OnReset() noexcept {}
	virtual void OnGui() noexcept {}
	virtual void OnParameterGui() noexcept {}

private:
	std::vector<SBar> mDailyBarList;
};