#pragma once

#include <string>
#include <map>
#include <iostream>
#include <curl/curl.h>
#include "field.h"
#include "filterutils.h"

template<typename Of>
struct Filter
{    
    virtual std::string serialize() = 0;
};

template<typename Of>
struct FilterList : public Filter<Of>
{
    FilterList(const std::initializer_list<std::shared_ptr<Filter<Of>>> &filters) : filters(filters) {}
    std::string filterName { "OR" };
    std::vector<std::shared_ptr<Filter<Of>>> filters;

    std::string serialize();
};

template <typename Of>
std::string FilterList<Of>::serialize()
{
    std::string result = "\"" + this->filterName + "\":[";

    size_t i = 1;
    for (const auto &el : this->filters)
    {
        result += el->serialize() + (i++ < filters.size() ? "," : "]");
    }
    return result;
}

template <typename Of, typename T>
struct ActualFilter: public Filter<Of>
{
    ActualFilter(Field<Of, T> f, T val, std::string op) : val(val), op(op), f(f) {}
    T val;
    std::string op;
    Field<Of, T> f;
    std::string serialize();
};

template <typename Of, typename T>
struct ActualFilter<Of, std::initializer_list<T>>: public Filter<Of>
{
    ActualFilter(Field<Of, T> f, const std::initializer_list<T> &val, std::string op) : val(val), op(op), f(f) {}
    std::initializer_list<T> val;
    std::string op;
    Field<Of, T> f;
    std::string serialize();
};

template<typename Of, typename T>
std::string ActualFilter<Of, T>::serialize()
{
    auto res = opLookup.find(this->op);
    if (res == opLookup.end())
    {
        throw "Op not found: " + this->op;
    }

    return "\"" + this->f.name + res->second + "\":" + printJsonValue(this->val);
}

template<typename Of, typename T>
std::string ActualFilter<Of, std::initializer_list<T>>::serialize()
{
    auto res = opLookup.find(this->op);
    if (res == opLookup.end())
    {
        throw "Op not found: " + this->op;
    }

    std::string result = "\"" + this->f.name + res->second + "\":[";

    size_t i = 1;
    for (const auto &el : this->val)
    {
        result += printJsonValue(el) + (i++ < val.size() ? "," : "]");
    }
    return result;
}
