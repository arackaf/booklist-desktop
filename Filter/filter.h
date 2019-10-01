#pragma once

#include <string>
#include <map>
#include <iostream>
#include <curl/curl.h>
#include <json.hpp>

#include "field.h"
#include "filterUtils.h"
#include "operatorOr.h"
#include "operatorLess.h"

template<typename Of>
struct Filter
{    
    using OfType = Of;
    virtual std::string serialize() = 0;
    virtual void addToSerialization(nlohmann::json &)  = 0;
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
    void addToSerialization(nlohmann::json &) override;
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

template <typename Of>
void OrFilter<Of>::addToSerialization(nlohmann::json &j)
{
}

template<typename Of>
struct AndFilter : public FilterList<Of>
{
    using FilterList<Of>::FilterList;
    std::string serialize() override;
    void addToSerialization(nlohmann::json &);
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

template <typename Of>
void AndFilter<Of>::addToSerialization(nlohmann::json &j)
{
}


