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
	for (size_t i = _begin; i < _end; ++i)
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

		ImColor color = bar.mOpen > bar.mClose ? ImColor(0, 180, 0) : ImColor(180, 0, 0);

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

	for (size_t i = _begin + 1; i < _end; ++i)
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

void ImPlotDrawColorRange(const std::vector<ImColor>& _valueList, float _yStart, float _yEnd)
{
	ImPlotDrawColorRange([&](size_t _index) { return _valueList[_index]; }, 0, _valueList.size(), _yStart, _yEnd);
}

void ImPlotDrawColorRange(SPlotGetColorFunc&& _getColorFunc, size_t _begin, size_t _end, float _yStart, float _yEnd)
{
	for (size_t i = _begin; i < _end; ++i)
	{
		ImColor color = _getColorFunc(i);

		ImVec2 pos1 = ImPlot::PlotToPixels(i + 0.0, 0.0);
		ImVec2 pos2 = ImPlot::PlotToPixels(i + 1.0, 0.0);

		ImVec2 clipMax = ImPlot::GetPlotDrawList()->GetClipRectMax();
		pos1.y = clipMax.y - _yStart;
		pos2.y = clipMax.y - _yEnd;

		ImPlot::GetPlotDrawList()->AddRectFilled(pos1, pos2, color);
	}
}

void ImPlotDrawZTLevel(const SZTLevel& _ztLevel)
{
	const std::vector<SZTPath>& pathList = _ztLevel.mPathList;
	const std::vector<SZTPolyLine>& polylineList = _ztLevel.mPolyLineList;

	//for (size_t i = 0; i != pathList.size(); ++i)
	//{
	//	const SZTPath& path = pathList[i];

	//	ImVec2 pos1 = ImPlot::PlotToPixels(path.mBarStart + 0.0, path.mPriceStart);
	//	ImVec2 pos2 = ImPlot::PlotToPixels(path.mBarEnd + 0.0, path.mPriceEnd);

	//	ImColor color = (path.GetTrend() == EZTTrend::Down) ? ImColor(0, 255, 0) : ImColor(255, 0, 0);

	//	ImPlot::GetPlotDrawList()->AddLine(pos1, pos2, color, 4);
	//}

	for (size_t i = 0; i != polylineList.size(); ++i)
	{
		const SZTPolyLine& polyline = polylineList[i];

		for (size_t j = polyline.mPathStart; j <= polyline.mPathEnd; ++j)
		{
			const SZTPath& path = pathList[j];

			ImVec2 pos1 = ImPlot::PlotToPixels(path.mBarStart + 0.0, path.mPriceStart);
			ImVec2 pos2 = ImPlot::PlotToPixels(path.mBarEnd + 0.0, path.mPriceEnd);

			ImColor color = (path.GetTrend() == EZTTrend::Down) ? ImColor(0, 255, 0) : ImColor(255, 0, 0);

			ImPlot::GetPlotDrawList()->AddLine(pos1, pos2, color, 2);
		}

		//ImVec2 pos1 = ImPlot::PlotToPixels(polyline.mBarStart + 0.0, polyline.mPriceStart);
		//ImVec2 pos2 = ImPlot::PlotToPixels(polyline.mBarEnd + 0.0, polyline.mPriceEnd);

		//ImColor color = (polyline.GetTrend() == EZTTrend::Up || polyline.GetTrend() == EZTTrend::ConsolidationUp) ? ImColor(255,0, 0) : ImColor(0, 255, 0);

		//ImPlot::GetPlotDrawList()->AddLine(pos1, pos2, color, 7);
	}
}
