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

		if (mDailyBarList.size() >= 2 && barList.empty() == false)
		{
			SCash cash = _context->GetCash();
			SPosition position = _context->GetPosition(mSymbol);

			double longMa[2] = { mLongMaList[mLongMaList.size() - 1], mLongMaList[mLongMaList.size() - 2] };

			double shortMa = mShortMaList.back();
			if (mClosePriceList.back() > shortMa && longMa[0] > longMa[1])
				_context->BuyVolume(mSymbol, CalcMaxPurchaseCount(cash.mBalance, barList[0].mOpen, 0.0015), barList[0].mOpen);
			else if (mClosePriceList.back() < shortMa)
				_context->SellVolume(mSymbol, position.mVolume, barList[0].mClose);
		}

		mDailyBarList.push_back(barList[0]);
		mClosePriceList.push_back(barList[0].mClose);
		mCashList.push_back(_context->GetCash());
		mShortMaList = CalcMa(mClosePriceList, mShortPeroid);
		mLongMaList = CalcMa(mClosePriceList, mLongPeroid);
		mZTContent = CalcZT(mDailyBarList, false);
	}

	void OnReset() noexcept override
	{
		mDailyBarList.clear();
		mClosePriceList.clear();
		mShortMaList.clear();
		mLongMaList.clear();
		mCashList.clear();
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
				if (ImPlot::BeginItem(u8"K线"/*, ImPlotItemFlags_NoLegend*/))
				{
					ImPlotDrawCandlestick(mDailyBarList);
					ImPlot::EndItem();
				}

				if (ImPlot::BeginItem(u8"短周期均线"))
				{
					ImPlotDrawCurve(mShortMaList, ImPlot::GetCurrentItemColor());
					ImPlot::EndItem();
				}

				if (ImPlot::BeginItem(u8"长周期均线"))
				{
					ImPlotDrawCurve(mLongMaList, ImPlot::GetCurrentItemColor());
					ImPlot::EndItem();
				}

				if (ImPlot::BeginItem(u8"笔", ImPlotItemFlags_NoLegend))
				{
					if (mZTContent.mLevelList.empty() == false)
						ImPlotDrawZTLevel(mZTContent.mLevelList[0]);
					ImPlot::EndItem();
				}

				if (ImPlot::BeginItem(u8"长周期均线趋势", ImPlotItemFlags_NoLegend))
				{
					ImPlotDrawColorRange([&](size_t _index)->ImColor
						{
							return mLongMaList[_index] > mLongMaList[_index - 1] ? ImColor(255, 223, 0) : ImColor(148, 0, 211);
						}, 1, mShortMaList.size(), 0, 5);
					ImPlot::EndItem();
				}

				if (ImPlot::BeginItem(u8"均线金叉死叉区域", ImPlotItemFlags_NoLegend))
				{
					ImPlotDrawColorRange([&](size_t _index)->ImColor
						{
							return (mLongMaList[_index] > mLongMaList[_index - 1] && mClosePriceList[_index] > mShortMaList[_index]) ? ImColor(255, 223, 0) : ImColor(148, 0, 211);
						}, 1, mShortMaList.size(), 5, 10);
					ImPlot::EndItem();
				}

				ImPlot::EndPlot();
			}

			if (ImPlot::BeginPlot(u8"资金曲线"))
			{
				ImPlot::SetupAxes(u8"时间", u8"金额", 0, ImPlotAxisFlags_RangeFit);
				ImPlot::SetupAxisFormat(ImAxis_Y1, "%.2f");
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
							return mCashList[_index].mBalance + mCashList[_index].mMarketValue;
						}, 0, mCashList.size(), ImPlot::GetCurrentItemColor());
					ImPlot::EndItem();
				}

				if (ImPlot::BeginItem(u8"买入躺平模式资金总额"))
				{
					ImPlotDrawCurve([&](size_t _index)->double
						{
							return mInitCash.mBalance * mClosePriceList[_index] / mClosePriceList[0];
						}, 0, mCashList.size(), ImPlot::GetCurrentItemColor());
					ImPlot::EndItem();
				}

				if (ImPlot::BeginItem(u8"长周期均线趋势", ImPlotItemFlags_NoLegend))
				{
					ImPlotDrawColorRange([&](size_t _index)->ImColor
						{
							return mLongMaList[_index] > mLongMaList[_index - 1] ? ImColor(255, 223, 0) : ImColor(148, 0, 211);
						}, 1, mShortMaList.size(), 0, 5);
					ImPlot::EndItem();
				}

				if (ImPlot::BeginItem(u8"均线金叉死叉区域", ImPlotItemFlags_NoLegend))
				{
					ImPlotDrawColorRange([&](size_t _index)->ImColor
						{
							return (mLongMaList[_index] > mLongMaList[_index - 1] && mClosePriceList[_index] > mShortMaList[_index]) ? ImColor(255, 223, 0) : ImColor(148, 0, 211);
						}, 1, mShortMaList.size(), 5, 10);
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
		if (ImGui::InputText(u8"标的", buffer, sizeof(buffer)))
			mSymbol = buffer;

		ImGui::InputInt(u8"短均线周期", &mShortPeroid);
		mShortPeroid = Math::Max(1, mShortPeroid);

		ImGui::InputInt(u8"长均线周期", &mLongPeroid);
		mLongPeroid = Math::Max(mShortPeroid, mLongPeroid);
	}

	void OnStatsGui() noexcept override
	{
		ImGui::TextWrapped(u8"在股价高于短周期均线，并且长周期均线向上时买入，当股价低于短周期均线时卖出");

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

		ImGui::Separator();

		if (mDailyBarList.size() >= 2)
		{
			double longMa[2] = { mLongMaList[mLongMaList.size() - 1], mLongMaList[mLongMaList.size() - 2] };

			double shortMa = mShortMaList.back();
			if (mClosePriceList.back() > shortMa && longMa[0] > longMa[1])
				ImGui::Text(u8"建议下个交易日建仓");
			else if (mClosePriceList.back() < shortMa)
				ImGui::Text(u8"建议下个交易日清仓");
		}
	}

private:
	std::vector<SBar> mDailyBarList;
	std::vector<double> mClosePriceList;
	std::vector<double> mShortMaList;
	std::vector<double> mLongMaList;
	SZTContent mZTContent;
	std::vector<SCash> mCashList;


	std::string mSymbol = "SHSE.000016";
	int mShortPeroid = 21;
	int mLongPeroid = 60;

	SCash mInitCash;

	Math::SDouble2 mXRange = Math::SDouble2(0, 1);
};
