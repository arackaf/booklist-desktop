#pragma once

#include <string>
#include "filter.h"

template <typename Of>
class Query {
public:
    Query(std::initializer_list<Filter<Of>> passedFilters)
    {
        this->filters = passedFilters;
    }
private:
    std::initializer_list<Filter<Of>> filters;
};
