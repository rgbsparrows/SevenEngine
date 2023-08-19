#include "GMQuantCore/Util/QuantUI.h"
#include "GMQuantCore/QuantStrategy.h"
#include "GMQuantCore/QuantStrategyContext.h"

#include "imgui.h"
#include "implot_internal.h"

class SMA21Strategy : public SQuantStrategyBase
{
	DECLARE_CLASSOBJECT_BODY(SMA21Strategy, SQuantStrategyBase)

public:
	std::vector<std::chrono::hh_mm_ss<std::chrono::seconds>> GetScheduleTimeList() const noexcept override
	{
		return {
			std::chrono::hh_mm_ss<std::chrono::seconds>(std::chrono::hours(20))
		};
	}

	void OnSchedule(IQuantStrategyContext* _context) noexcept override
	{
		std::vector<SBar> barList = _context->GetLastestNBar(mSymbol, 1);
		if (barList.empty() == false)
			mDailyBarList.push_back(barList[0]);
		mTimeList.push_back(_context->GetNowTime());
	}

	void OnReset() noexcept override
	{
		mDailyBarList.clear();
		mTimeList.clear();
	}

	void OnGui() noexcept override
	{
		ImGui::PushID(this);
		if (ImPlot::BeginPlot(u8"KÏßÍ¼Plot"))
		{

			if (ImPlot::BeginItem(u8"KÏßÍ¼"))
			{
				ImPlotDrawBar(mDailyBarList);
				ImPlot::EndItem();
			}
			ImPlot::EndPlot();
		}

		ImGui::PopID();
	}
	void OnParameterGui() noexcept override
	{
		char buffer[256];
		strcpy_s(buffer, mSymbol.c_str());
		if (ImGui::InputText("Symbol", buffer, sizeof(buffer)))
			mSymbol = buffer;
	}

	void OnStatsGui() noexcept override
	{
		if (mTimeList.empty() == false)
		{
			std::chrono::system_clock::time_point time = mTimeList.back() + std::chrono::hours(8);
			std::chrono::year_month_day ymd = std::chrono::floor<std::chrono::days>(time);
			std::chrono::hh_mm_ss<std::chrono::seconds> hms(std::chrono::floor<std::chrono::seconds>(time) - std::chrono::floor<std::chrono::days>(time));

			ImGui::Text(u8"Time: %s", std::format("{:%F} {:%T}", ymd, hms).c_str());
		}

		if (mDailyBarList.empty() == false)
		{
			SBar bar = mDailyBarList.back();

			ImGui::Text(u8"Open: %.2f", bar.mOpen);
			ImGui::Text(u8"High: %.2f", bar.mHigh);
			ImGui::Text(u8"Low: %.2f", bar.mLow);
		}
	}

private:
	std::vector<SBar> mDailyBarList;
	std::vector<std::chrono::system_clock::time_point> mTimeList;

	std::string mSymbol = "SHSE.000001";
};
