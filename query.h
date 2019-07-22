#pragma once

#include <string>
#include "filter.h"

template <typename Of>
class Query {
public:
    template<typename ...Ts>
    Query(std::initializer_list<Ts...> passedFilters)
    {
        this->filters = passedFilters;
    }
private:
    std::initializer_list<Filter<Of>> filters;
};
