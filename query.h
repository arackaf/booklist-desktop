#pragma once

#include <string>
#include "filter.h"

template <typename Of>
class Query {
public:
    Query(std::initializer_list<std::shared_ptr<Filter<Of>>> passedFilters) : filters(passedFilters) {}
    std::vector<std::shared_ptr<Filter<Of>>> filters;
};
