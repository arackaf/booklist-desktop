#pragma once

#include "filter.h"
#include "actualFilter.h"
#include "field.h"

template <typename Of, typename T, typename U, typename = std::enable_if_t<std::is_arithmetic_v<U>>>
decltype (auto) operator<(const Field<Of, T> &f, const U &val)
{
    return val < f;
}

template <typename Of, typename T, typename U, typename = std::enable_if_t<std::is_arithmetic_v<U>>>
decltype (auto) operator<(const U &val, const Field<Of, T> &f)
{
    using AdjustedType = std::common_type_t<T, U>;
    using FilterType = ActualFilter<Of, AdjustedType>;

    return std::make_shared<FilterType>(FilterType{ f, static_cast<AdjustedType>(val), "<" });
}
