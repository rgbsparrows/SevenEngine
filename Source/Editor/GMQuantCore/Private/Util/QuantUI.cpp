#include "GMQuantCore/Util/QuantUI.h"

#include "imgui.h"
#include "implot.h"
#include "implot_internal.h"

void ImPlotDrawBar(std::vector<SBar>& _barList)
{
	for (size_t i = 0; i != _barList.size(); ++i)
	{
		ImPlot::FitPointX(i - 0.25);
		ImPlot::FitPointX(i + 0.25);
		ImPlot::FitPointY(_barList[i].mLow);
		ImPlot::FitPointY(_barList[i].mHigh);
	}

	for (size_t i = 0; i != _barList.size(); ++i)
	{
		ImVec2 openPos = ImPlot::PlotToPixels(i - 0.25, _barList[i].mOpen);
		ImVec2 closePos = ImPlot::PlotToPixels(i + 0.25, _barList[i].mClose);

		ImVec2 lowPos = ImPlot::PlotToPixels(i + 0.0, _barList[i].mLow);
		ImVec2 highPos = ImPlot::PlotToPixels(i + 0.0, _barList[i].mHigh);

		ImColor color = _barList[i].mOpen > _barList[i].mClose ? ImColor(0, 255, 0) : ImColor(255, 0, 0);

		if (openPos.x < ImPlot::GetPlotDrawList()->GetClipRectMin().x || closePos.x > ImPlot::GetPlotDrawList()->GetClipRectMax().x)
			continue;

		ImPlot::GetPlotDrawList()->AddRectFilled(closePos, openPos, color);
		ImPlot::GetPlotDrawList()->AddLine(lowPos, highPos, color);
	}
}
