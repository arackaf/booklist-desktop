#pragma once

#include <string>
#include <map>
#include <iostream>
#include <curl/curl.h>
#include "field.h"

template<typename Of>
struct Filter
{
    Filter(Field<Of> f, std::string op) : f(f), op(op) {}
    Field<Of> f;
    std::string op;
    virtual std::string serialize() = 0; //dummy POC method
};

template <typename Of, typename T>
struct ActualFilter: public Filter<Of>
{
    ActualFilter(Field<Of> f, T val, std::string op) : Filter<Of>(f, op), val(val) {}
    T val;
    virtual std::string serialize();
};

template <typename Of, typename T>
struct ActualFilter<Of, std::initializer_list<T>>: public Filter<Of>
{
    ActualFilter(Field<Of> f, T val, std::string op) : Filter<Of>(f, op), val(val) {}
    T val;
    virtual std::string serialize();
};

template <typename T>
std::string printJsonValue(T val);

template <typename T>
std::string printJsonValue(T val)
{
    return std::to_string(val);
}

template <>
std::string printJsonValue<std::string>(std::string);

const extern std::map<std::string, std::string> opLookup;


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

    std::string result = "\"" + this->f.name + res->second + "\":";

    for (const auto &el : this->val)
    {
        result += printJsonValue(el);
    }
    return result;
}
