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
    virtual std::string serialize() = 0;

    virtual std::shared_ptr<Filter<Of>> operator||(const std::shared_ptr<Filter<Of>>);
};

template<typename Of>
std::shared_ptr<Filter<Of>> Filter<Of>::operator||(const std::shared_ptr<Filter<Of>>){}

template<typename Of>
struct OrFilter : public Filter<Of>
{
    OrFilter(const std::initializer_list<std::shared_ptr<Filter<Of>>> &filters) : filters(filters) {}
    std::vector<std::shared_ptr<Filter<Of>>> filters;

    std::string filterName { "OR" };
    std::string serialize();
    std::shared_ptr<Filter<Of>> operator||(const std::shared_ptr<Filter<Of>> &rhs);
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
std::shared_ptr<Filter<Of>> OrFilter<Of>::operator||(const std::shared_ptr<Filter<Of>> &rhs)
{
    return std::make_shared<Filter<Of>>(OrFilter<Of>{ std::initializer_list<Filter<Of>> { ActualFilter<Of, int>{ Field<Of, int>{"foo"}, 12, "==" } } } );
}

template<typename Of>
struct AndFilter : public Filter<Of>
{
    AndFilter(const std::initializer_list<std::shared_ptr<Filter<Of>>> &filters) : filters(filters) {}
    std::vector<std::shared_ptr<Filter<Of>>> filters;

    std::string serialize();
    std::shared_ptr<Filter<Of>> operator||(const std::shared_ptr<Filter<Of>> &rhs);
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
std::shared_ptr<Filter<Of>> AndFilter<Of>::operator||(const std::shared_ptr<Filter<Of>> &rhs)
{
    return std::make_shared<AndFilter<Of>>(AndFilter<Of>{});
}

template <typename Of, typename T>
struct ActualFilter: public Filter<Of>
{
    ActualFilter(Field<Of, T> f, T val, std::string op) : val(val), op(op), f(f) {}
    T val;
    std::string op;
    Field<Of, T> f;
    std::string serialize();
    std::shared_ptr<Filter<Of>> operator||(const std::shared_ptr<Filter<Of>> &rhs);
};

template <typename Of, typename T>
std::shared_ptr<Filter<Of>> ActualFilter<Of, T>::operator||(const std::shared_ptr<Filter<Of>> &rhs)
{
    return std::make_shared<ActualFilter<Of, T>>(ActualFilter<Of, T>{});
}

template <typename Of, typename T>
struct ActualFilter<Of, std::initializer_list<T>>: public Filter<Of>
{
    ActualFilter(Field<Of, T> f, const std::initializer_list<T> &val, std::string op) : val(val), op(op), f(f) {}
    std::initializer_list<T> val;
    std::string op;
    Field<Of, T> f;
    std::string serialize();
    std::shared_ptr<Filter<Of>> operator||(const std::shared_ptr<Filter<Of>> &rhs);
};

template <typename Of, typename T>
std::shared_ptr<Filter<Of>> ActualFilter<Of, std::initializer_list<T>>::operator||(const std::shared_ptr<Filter<Of>> &rhs)
{
    return std::make_shared<ActualFilter<Of, T>>(ActualFilter<Of, T>{});
}

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
