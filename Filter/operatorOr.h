#pragma once

#include "filter.h"
#include "actualFilter.h"

template <typename Of, typename T>
std::shared_ptr<OrFilter<Of>> operator ||(const std::shared_ptr<ActualFilter<Of, T>> &lhs, const std::shared_ptr<OrFilter<Of>> &rhs)
{
    return rhs || lhs;
}


template <typename Of, typename T>
std::shared_ptr<OrFilter<Of>> operator ||(const std::shared_ptr<OrFilter<Of>> &lhs, const std::shared_ptr<ActualFilter<Of, T>> &rhs)
{
    lhs->filters.push_back(rhs);
    return lhs;
}

template
<
    typename FilterLHS,
    typename FilterRHS,
    typename Of = typename FilterLHS::OfType,
    typename = std::enable_if_t<std::is_base_of_v<Filter<Of>, FilterLHS> && std::is_base_of_v<Filter<Of>, FilterRHS>>
>
std::shared_ptr<OrFilter<Of>> operator ||(const std::shared_ptr<FilterLHS> &lhs, const std::shared_ptr<FilterRHS> &rhs)
{
    return std::make_shared<OrFilter<Of>>(OrFilter<Of>{ lhs, rhs });
}
