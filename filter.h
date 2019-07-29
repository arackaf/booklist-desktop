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
    Filter(std::string op) : op(op) {}
    std::string op;
    virtual std::string serialize() = 0; //dummy POC method
};

template <typename Of, typename T>
struct ActualFilter: public Filter<Of>
{
    ActualFilter(Field<Of, T> f, T val, std::string op) : Filter<Of>(op), val(val), f(f) {}
    T val;
    Field<Of, T> f;
    virtual std::string serialize();
};

template <typename Of, typename T>
struct ActualFilter<Of, std::initializer_list<T>>: public Filter<Of>
{
    ActualFilter(Field<Of, T> f, const std::initializer_list<T> &val, std::string op) : Filter<Of>(op), val(val), f(f) {}
    std::initializer_list<T> val;
    Field<Of, T> f;
    virtual std::string serialize();
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
