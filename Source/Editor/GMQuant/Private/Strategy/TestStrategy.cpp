#include "GMQuantCore/QuantStrategy.h"
#include "GMQuantCore/Util/QuantMisc.h"
#include "GMQuantCore/QuantStrategyContext.h"

#include <format>
#include "imgui.h"
#include "implot.h"
#include "implot_internal.h"
#include "GMQuantCore/Util/QuantUI.h"

class STestStrategy : public SQuantStrategyBase
{
	DECLARE_CLASSOBJECT_BODY(STestStrategy, SQuantStrategyBase)

public:
	void OnInit(IQuantStrategyContext* _context) noexcept override
	{
		std::vector<std::vector<SBar>> KLineList;
		SBar InitBar;
		InitBar.mOpen = InitBar.mLow = 10;
		InitBar.mHigh = InitBar.mClose = 20;
		for (size_t i = 0; i != (1 << 10); ++i)
		{
			std::vector<SBar> barList;

			SBar InitUpBar;
			InitUpBar.mOpen = InitUpBar.mLow = 10;
			InitUpBar.mHigh = InitUpBar.mClose = 20;

			SBar InitDownBar;
			InitDownBar.mOpen = InitDownBar.mHigh = 20;
			InitDownBar.mClose = InitDownBar.mLow = 10;
			
			if (i & 1)
				barList.push_back(InitUpBar);
			else
				barList.push_back(InitDownBar);

			for (size_t j = 1; j != 10; ++j)
			{
				if ((i >> j) & 1)
					barList.push_back(MoveUpBar(barList.back()));
				else
					barList.push_back(MoveDownBar(barList.back()));
			}
			KLineList.push_back(barList);
		}

		for (size_t i = 0; i != KLineList.size(); ++i)
		{
			SBar emptyBar;
			emptyBar.mClose = emptyBar.mOpen = emptyBar.mHigh = emptyBar.mLow = 100;
			mBarList.push_back(emptyBar);
			mBarList.push_back(emptyBar);
			mBarList.push_back(emptyBar);
			mBarList.push_back(emptyBar);
			mBarList.insert(mBarList.end(), KLineList[i].begin(), KLineList[i].end());
		}

	}

	SBar  MoveUpBar(SBar _bar)
	{
		SBar newBar;
		newBar.mOpen = newBar.mLow = _bar.mLow + 5;
		newBar.mHigh = newBar.mClose = _bar.mHigh + 5;
		return newBar;
	}

	SBar MoveDownBar(SBar _bar)
	{
		SBar newBar;
		newBar.mOpen = newBar.mHigh = _bar.mHigh - 5;
		newBar.mClose = newBar.mLow = _bar.mLow - 5;
		return newBar;
	}

	void OnGui() noexcept override
	{
		if (ImPlot::BeginPlot(u8"KÏßÐÎÌ¬"))
		{
			if (ImPlot::BeginItem(u8"KÏß"))
			{
				ImPlotDrawCandlestick(mBarList);
				ImPlot::EndItem();
			}

			ImPlot::EndPlot();
		}
		
	}

	void OnStatsGui() noexcept override
	{
	}

private:

	std::vector<SBar> mBarList;
};
