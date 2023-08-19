#include "GMQuantCore/Util/QuantUI.h"

#include "imgui.h"
#include "implot.h"
#include "implot_internal.h"

void ImPlotDrawCandlestick(const std::vector<SBar>& _barList)
{
	ImPlotDrawCandlestick([&](size_t _index) { return _barList[_index]; }, 0, _barList.size());
}

void ImPlotDrawCandlestick(SPlotGetBarFunc&& _getBarFunc, size_t _begin, size_t _end)
{
	for (size_t i = _begin; i != _end; ++i)
	{
		SBar bar = _getBarFunc(i);

		if (ImPlot::FitThisFrame()) 
		{
			ImPlot::FitPointX(i - 0.25);
			ImPlot::FitPointX(i + 0.25);
			ImPlot::FitPointY(bar.mLow);
			ImPlot::FitPointY(bar.mHigh);
		}

		ImVec2 openPos = ImPlot::PlotToPixels(i - 0.25, bar.mOpen);
		ImVec2 closePos = ImPlot::PlotToPixels(i + 0.25, bar.mClose);

		ImVec2 lowPos = ImPlot::PlotToPixels(i + 0.0, bar.mLow);
		ImVec2 highPos = ImPlot::PlotToPixels(i + 0.0, bar.mHigh);

		ImColor color = bar.mOpen > bar.mClose ? ImColor(0, 255, 0) : ImColor(255, 0, 0);

		ImPlot::GetPlotDrawList()->AddRectFilled(closePos, openPos, color);
		ImPlot::GetPlotDrawList()->AddLine(lowPos, highPos, color);
	}
}

void ImPlotDrawCurve(const std::vector<double>& _valueList, ImColor _color, float _thickness)
{
	ImPlotDrawCurve([&](size_t _index) { return _valueList[_index]; }, 0, _valueList.size(), _color, _thickness);
}

void ImPlotDrawCurve(SPlotGetValueFunc&& _getValueFunc, size_t _begin, size_t _end, ImColor _color, float _thickness)
{
	if (_end - _begin <= 1)
		return;

	if (ImPlot::FitThisFrame())
		ImPlot::FitPointY(_getValueFunc(_begin));

	for (size_t i = _begin + 1; i != _end; ++i)
	{
		double value1 = _getValueFunc(i - 1);
		double value2 = _getValueFunc(i);

		if (ImPlot::FitThisFrame())
		{
			ImPlot::FitPointX(i + 0.0);
			ImPlot::FitPointY(value2);
		}

		ImVec2 pos1 = ImPlot::PlotToPixels(i - 1.0, value1);
		ImVec2 pos2 = ImPlot::PlotToPixels(i + 0.0, value2);

		ImPlot::GetPlotDrawList()->AddLine(pos1, pos2, _color, _thickness);
	}
}

void ImPlotDrawColorRange(const std::vector<ImColor>& _valueList, float _thickness)
{
	ImPlotDrawColorRange([&](size_t _index) { return _valueList[_index]; }, 0, _valueList.size(), _thickness);
}

void ImPlotDrawColorRange(SPlotGetColorFunc&& _getColorFunc, size_t _begin, size_t _end, float _thickness)
{
	for (size_t i = _begin; i != _end; ++i)
	{
		ImColor color = _getColorFunc(i);

		ImVec2 pos1 = ImPlot::PlotToPixels(i + 0.0, 0.0);
		ImVec2 pos2 = ImPlot::PlotToPixels(i + 1.0, 0.0);

		ImVec2 clipMax = ImPlot::GetPlotDrawList()->GetClipRectMax();
		pos1.y = clipMax.y;
		pos2.y = clipMax.y - _thickness;

		ImPlot::GetPlotDrawList()->AddRectFilled(pos1, pos2, color);
	}
}
