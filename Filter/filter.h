#pragma once

#include <string>
#include <map>
#include <iostream>
#include <curl/curl.h>
#include "field.h"
#include "filterutils.h"

template <typename Of, typename T>
struct Field;

template <typename Of, typename T>
struct NumericField;

template<typename Of>
struct Filter
{    
    using OfType = Of;
    virtual std::string serialize() = 0;
    virtual ~Filter(){}
};

template<typename Of>
struct FilterList : public Filter<Of>
{
    FilterList(const std::initializer_list<std::shared_ptr<Filter<Of>>> &filters) : filters(filters) {}
    std::vector<std::shared_ptr<Filter<Of>>> filters;
};

template<typename Of>
struct OrFilter : public FilterList<Of>
{
    using FilterList<Of>::FilterList;
    std::string filterName { "OR" };
    std::string serialize() override;
};

template <typename Of>
std::string OrFilter<Of>::serialize()
{
    std::string result = "\"" + this->filterName + "\":[";

    size_t i = 1;
    for (const auto &el : this->filters)
    {
        result += "{" + el->serialize() + "}" + (i++ < this->filters.size() ? "," : "]");
    }
    return result;
}

template<typename Of>
struct AndFilter : public FilterList<Of>
{
    using FilterList<Of>::FilterList;
    std::string serialize() override;
};

template <typename Of>
std::string AndFilter<Of>::serialize()
{
    std::string result { "" };

    size_t i = 1;
    for (const auto &el : this->filters)
    {
        result += el->serialize() + (i++ < this->filters.size() ? "," : "");
    }
    return result;
}


