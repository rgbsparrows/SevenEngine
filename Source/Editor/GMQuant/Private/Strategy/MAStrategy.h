#include "GMQuantCore/Util/QuantUI.h"
#include "GMQuantCore/QuantStrategy.h"
#include "GMQuantCore/Util/QuantMath.h"
#include "GMQuantCore/Util/QuantMisc.h"
#include "GMQuantCore/QuantStrategyContext.h"

#include "imgui.h"
#include "implot_internal.h"

class SMAStrategy : public SQuantStrategyBase
{
	DECLARE_CLASSOBJECT_BODY(SMAStrategy, SQuantStrategyBase)

public:
	std::vector<std::chrono::hh_mm_ss<std::chrono::seconds>> GetScheduleTimeList() const noexcept override
	{
		return {
			std::chrono::hh_mm_ss<std::chrono::seconds>(std::chrono::hours(20))
		};
	}

	void OnInit(IQuantStrategyContext* _context) noexcept override
	{
		mInitCash = _context->GetCash();
	}

	void OnSchedule(IQuantStrategyContext* _context) noexcept override
	{
		std::vector<SBar> barList = _context->GetLastestNBar(mSymbol, 1);

		if (barList.empty())
			return;

		if (mMaList.empty() == false && barList.empty() == false)
		{
			SCash cash = _context->GetCash();
			SPosition position = _context->GetPosition(mSymbol);

			double ma = mMaList.back();
			if (mClosePriceList.back() > ma)
				_context->BuyVolume(mSymbol, CalcMaxPurchaseCount(cash.mBalance, barList[0].mOpen, 0.0015), barList[0].mOpen);
			else if (mClosePriceList.back() < ma)
				_context->SellVolume(mSymbol, position.mVolume, barList[0].mClose);
		}

		mDailyBarList.push_back(barList[0]);
		mClosePriceList.push_back(barList[0].mClose);
		mCashList.push_back(_context->GetCash());
		mMaList = CalcMa(mClosePriceList, mPeroid);
	}

	void OnReset() noexcept override
	{
		mDailyBarList.clear();
		mClosePriceList.clear();
		mMaList.clear();
	}

	void OnGui() noexcept override
	{
		ImGui::PushID(this);

		if (ImPlot::BeginAlignedPlots("AlignedGroup")) {
			if (ImPlot::BeginPlot(u8"K线图"))
			{
				ImPlot::SetupAxes(u8"时间", nullptr, 0, ImPlotAxisFlags_RangeFit);
				ImPlot::SetupAxisLinks(ImAxis_X1, &mXRange[0], &mXRange[1]);
				ImPlot::SetupAxisFormat(ImAxis_X1, [&](double value, char* buff, int size, void* user_data) -> int
					{
						buff[0] = '\0';
						size_t valueInt = static_cast<size_t>(value);
						if (valueInt > 0 && valueInt < mDailyBarList.size())
						{
							std::chrono::system_clock::time_point time = mDailyBarList[valueInt].mEob;
							std::string data = FormatTime_yy_mm_dd(ConvertToYMD(time));
							memcpy_s(buff, size, data.c_str(), data.size() + 1);
						}
						return 0;
					});
				if (ImPlot::BeginItem(u8"K线"))
				{
					ImPlotDrawCandlestick(mDailyBarList);
					ImPlot::EndItem();
				}

				if (ImPlot::BeginItem(u8"均线"))
				{
					ImPlotDrawCurve(mMaList, ImPlot::GetCurrentItemColor());
					ImPlot::EndItem();
				}

				if (ImPlot::BeginItem(u8"均线金叉死叉区域", ImPlotItemFlags_NoLegend))
				{
					ImPlotDrawColorRange([&](size_t _index)->ImColor
						{
							if (_index > 0 && _index < mMaList.size())
								return mMaList[_index] < mClosePriceList[_index] ? ImColor(255, 223, 0) : ImColor(148, 0, 211);

							return ImColor();
						}, 0, mMaList.size());
					ImPlot::EndItem();
				}

				ImPlot::EndPlot();
			}

			if (ImPlot::BeginPlot(u8"资金曲线"))
			{
				ImPlot::SetupAxes(u8"时间", u8"金额", 0, ImPlotAxisFlags_RangeFit);
				if (mUseLogAxisForCash)
					ImPlot::SetupAxisScale(ImAxis_Y1, ImPlotScale_Log10);
				ImPlot::SetupAxisLinks(ImAxis_X1, &mXRange[0], &mXRange[1]);
				ImPlot::SetupAxisFormat(ImAxis_X1, [&](double value, char* buff, int size, void* user_data) -> int
					{
						buff[0] = '\0';
						size_t valueInt = static_cast<size_t>(value);
						if (valueInt > 0 && valueInt < mDailyBarList.size())
						{
							std::chrono::system_clock::time_point time = mDailyBarList[valueInt].mEob;
							std::string data = FormatTime_yy_mm_dd(ConvertToYMD(time));
							memcpy_s(buff, size, data.c_str(), data.size() + 1);
						}
						return 0;
					});

				if (ImPlot::BeginItem(u8"资金总额"))
				{
					ImPlotDrawCurve([&](size_t _index)->double
						{
							if (_index >= 0 && _index < mCashList.size())
								return mCashList[_index].mBalance + mCashList[_index].mMarketValue;
							return 0;
						}, 0, mCashList.size(), ImPlot::GetCurrentItemColor());
					ImPlot::EndItem();
				}

				if (ImPlot::BeginItem(u8"均线金叉死叉区域", ImPlotItemFlags_NoLegend))
				{
					ImPlotDrawColorRange([&](size_t _index)->ImColor
						{
							if (_index > 0 && _index < mMaList.size())
								return mMaList[_index] < mClosePriceList[_index] ? ImColor(255, 223, 0) : ImColor(148, 0, 211);

							return ImColor();
						}, 0, mMaList.size());
					ImPlot::EndItem();
				}

				ImPlot::EndPlot();
			}
			ImPlot::EndAlignedPlots();
		}
		ImGui::PopID();
	}
	void OnParameterGui() noexcept override
	{
		char buffer[256];
		strcpy_s(buffer, mSymbol.c_str());
		if (ImGui::InputText("标的", buffer, sizeof(buffer)))
			mSymbol = buffer;

		ImGui::InputInt(u8"均线周期", &mPeroid);
		mPeroid = Math::Max(0, mPeroid);

		ImGui::Checkbox(u8"使用对数坐标", &mUseLogAxisForCash);
	}

	void OnStatsGui() noexcept override
	{
		if (mDailyBarList.empty() == false)
		{
			SBar bar = mDailyBarList.back();

			ImGui::Text(u8"初始资金: %.2f", mInitCash.mBalance + mInitCash.mMarketValue);
			ImGui::Text(u8"现有资金: %.2f", mCashList.back().mBalance + mCashList.back().mMarketValue);
		}
		else
		{
			ImGui::Text(u8"标的暂未上市");
		}
	}

private:
	std::vector<SBar> mDailyBarList;
	std::vector<double> mClosePriceList;
	std::vector<double> mMaList;
	std::vector<SCash> mCashList;

	std::string mSymbol = "SHSE.000016";
	int mPeroid = 21;
	
	SCash mInitCash;
	bool mUseLogAxisForCash = true;

	Math::SDouble2 mXRange = Math::SDouble2(0, 1);
};
