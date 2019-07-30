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

template<typename ...FilterArgs>
decltype (auto) makeFilter(FilterArgs... filters)
{
    using Tuple = decltype(std::make_tuple(filters...));
    using Of = typename FilterType<std::tuple_element_t<0, Tuple>>::Type;

    return Query<Of>{{ filters... }};
}
