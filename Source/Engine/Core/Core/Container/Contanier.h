#pragma once

#include <tuple>
#include <vector>

template<typename ..._elementTypes>
using PackageVector = std::vector<std::tuple<_elementTypes...>>;
