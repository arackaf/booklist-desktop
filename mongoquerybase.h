#pragma once

#include "query.h"
#include "filter.h"

template <typename Of, typename T>
std::shared_ptr<Filter<Of>> operator==(Field<Of> f, const T &val)
{
    return val == f;
}

template <typename Of, typename T>
std::shared_ptr<Filter<Of>> operator==(const T &val, const Field<Of> &f)
{
    return std::make_shared<ActualFilter<Of, T>>(ActualFilter<Of, T>{ f, val, "==" });
}

template <typename Of, size_t N>
std::shared_ptr<Filter<Of>> operator==(Field<Of> f, const char (&val) [N])
{
    return val == f;
}

template <typename Of, size_t N>
std::shared_ptr<Filter<Of>> operator==(const char (&val) [N], Field<Of> f)
{
    return std::make_shared<ActualFilter<Of, std::string>>(ActualFilter<Of, std::string>{ f, val, "==" });
}

template <typename T>
struct FilterType;

template <typename T>
struct FilterType<std::shared_ptr<Filter<T>>>
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
