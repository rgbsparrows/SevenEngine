#pragma once

#include <vector>
#include <tuple>

template<typename ..._elementTypes>
using PackageVector = std::vector<std::tuple<_elementTypes...>>;
