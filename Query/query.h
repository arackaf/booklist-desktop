#pragma once

#include <string>
#include "filter.h"

template<typename T>
std::string encode(T val);

template <>
std::string encode (std::string val);

template <typename Of>
class Query {
public:
    Query(std::initializer_list<std::shared_ptr<Filter<Of>>> passedFilters) : filters(passedFilters) {}
    std::vector<std::shared_ptr<Filter<Of>>> filters;

    std::string serialize();
};

template <typename Of>
std::string Query<Of>::serialize()
{
    std::string result{};

    for (size_t i = 0; i < filters.size(); i++)
    {
        if (i > 0)
        {
            result += ",";
        }
        result += filters[i]->serialize();
    }
    result = "{" + result + "}";
    return encode(result);
}
