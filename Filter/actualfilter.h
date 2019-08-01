#pragma once

#include "field.h"
#include "filter.h"
#include "filterutils.h"

template <typename Of>
struct Filter;

template <typename Of>
struct OrFilter;

template <typename Of, typename T>
struct Field;

template <typename Of, typename T>
struct NumericField;

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

//            OPERATOR ||

template <typename Of, typename T, typename U>
std::shared_ptr<OrFilter<Of>> operator ||(const std::shared_ptr<ActualFilter<Of, T>> &lhs, const std::shared_ptr<ActualFilter<Of, U>> &rhs)
{
    return std::make_shared<OrFilter<Of>>(OrFilter<Of>{ lhs, rhs });
}

//            OPERATOR <

template <typename Of, typename T, typename U>
decltype (auto) operator<(const NumericField<Of, T> &f, const U &val)
{
    return val < f;
}

template <typename Of, typename T, typename U>
decltype (auto) operator<(const U &val, const NumericField<Of, T> &f)
{
    using AdjustedType = std::common_type_t<T, U>;
    using FilterType = ActualFilter<Of, AdjustedType>;

    return std::make_shared<FilterType>(FilterType{ f, static_cast<AdjustedType>(val), "<" });
}

