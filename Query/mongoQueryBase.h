#pragma once

#include "query.h"
#include "filter.h"

template <typename T>
struct FilterType;

template <typename T>
struct FilterType<std::shared_ptr<Filter<T>>>
{
    using Type = T;
};

template <typename T, typename Of>
struct FilterType<std::shared_ptr<ActualFilter<T, Of>>>
{
    using Type = T;
};

template <typename T>
struct FilterType<std::shared_ptr<OrFilter<T>>>
{
    using Type = T;
};

template <typename T>
struct FilterType<std::shared_ptr<AndFilter<T>>>
{
    using Type = T;
};

template <typename T>
struct ExtractBaseFilterType{
    using FilterType = Filter<typename FilterType<T>::Type>;
};

template <typename T>
using ExtractBaseFilterType_t = typename ExtractBaseFilterType<T>::FilterType;

template<typename Of>
Query<Of> makeFilter()
{
    return Query<Of> {};
}


template<typename ...FilterArgs>
decltype (auto) makeFilter(FilterArgs... filters)
{
    using Tuple = decltype(std::make_tuple(filters...));
    using Of = typename FilterType<std::tuple_element_t<0, Tuple>>::Type;

    static_assert((... && std::is_same_v<Filter<Of>, ExtractBaseFilterType_t<FilterArgs>>), "Make sure all filters passed to makeFilter are for the same base type");

    return Query<Of>{{ filters... }};
}