#pragma once

#include "GMQuantCore/Util/QuantMath.h"
#include "GMQuantCore/QuantType.h"

#include "imgui.h"
#include <vector>
#include <functional>

using SPlotGetBarFunc = std::function<SBar(size_t)>;
using SPlotGetValueFunc = std::function<double(size_t)>;
using SPlotGetColorFunc = std::function<ImColor(size_t)>;

void ImPlotDrawCandlestick(const std::vector<SBar>& _barList);
void ImPlotDrawCandlestick(SPlotGetBarFunc&& _getBarFunc, size_t _begin, size_t _end);

void ImPlotDrawCurve(const std::vector<double>& _valueList, ImColor _color = ImColor(255, 255, 255), float _thickness = 1.5f);
void ImPlotDrawCurve(SPlotGetValueFunc&& _getValueFunc, size_t _begin, size_t _end, ImColor _color = ImColor(255, 255, 255), float _thickness = 1.5f);

void ImPlotDrawColorRange(const std::vector<ImColor>& _valueList, float _yStart = 0.f, float _yEnd = 5.f);
void ImPlotDrawColorRange(SPlotGetColorFunc&& _getColorFunc, size_t _begin, size_t _end, float _yStart = 0.f, float _yEnd = 5.f);

void ImPlotDrawZTLevel(const SZTLevel& _ztLevel);
