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
    std::string serialize();
    virtual void addToSerialization(nlohmann::json &)  = 0;
    virtual ~Filter(){}
};

template<typename Of>
std::string Filter<Of>::serialize()
{
    nlohmann::json j;
    this->addToSerialization(j);
    return j.dump();
}

template<typename Of>
void to_json(nlohmann::json &j, const std::shared_ptr<Filter<Of>> &f)
{
    f->addToSerialization(j);
}

template<typename Of>
struct FilterList : public Filter<Of>
{
    FilterList(const std::initializer_list<std::shared_ptr<Filter<Of>>> &filters) : filters(filters) {}
    std::vector<std::shared_ptr<Filter<Of>>> filters;
    virtual void addToSerialization(nlohmann::json &) = 0;
};

template<typename Of>
struct OrFilter : public FilterList<Of>
{
    using FilterList<Of>::FilterList;
    std::string filterName { "OR" };
    void addToSerialization(nlohmann::json &) override;
};

template <typename Of>
void OrFilter<Of>::addToSerialization(nlohmann::json &j)
{
    j[this->filterName] = this->filters;
}

template<typename Of>
struct AndFilter : public FilterList<Of>
{
    using FilterList<Of>::FilterList;
    void addToSerialization(nlohmann::json &) override;
};

template <typename Of>
void AndFilter<Of>::addToSerialization(nlohmann::json &j)
{
    for (auto i = 0; i < this->filters.size(); i++)
    {
        this->filters[i]->addToSerialization(j);
    }
}


