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

private:
    static std::map<std::string, std::string> opLookup;
};

template <typename T>
std::string serializeVal(T val);

template <typename T>
std::string serializeVal(T val)
{
    return std::to_string(val);
}

template <>
std::string serializeVal<std::string>(std::string);

template <typename Of, typename T>
std::map<std::string, std::string> ActualFilter<Of, T>::opLookup = std::map<std::string, std::string>
{
    { "==", "" },
    { "<", "_lt" },
    { "<=", "_lte" },
    { ">", "_gt" },
    { ">=", "_gte" }
};


template<typename Of, typename T>
std::string ActualFilter<Of, T>::serialize()
{
    auto res = opLookup.find(this->op);
    if (res == opLookup.end())
    {
        throw "Op not found: " + this->op;
    }

    return "\"" + this->f.name + res->second + "\":" + serializeVal(this->val);
}


