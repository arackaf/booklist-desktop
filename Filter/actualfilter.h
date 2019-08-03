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

template <typename Of, typename FieldType, typename FilterValueType = FieldType>
struct ActualFilter: public Filter<Of>
{
    ActualFilter(Field<Of, FieldType> f, FilterValueType val, std::string op) : val(val), op(op), f(f) {}
    FilterValueType val;
    std::string op;
    Field<Of, FieldType> f;
    std::string serialize() override;
};

template <typename Of, typename FieldType, typename FilterValueType = FieldType>
ActualFilter(Field<Of, FieldType>, FilterValueType, std::string) -> ActualFilter<Of, FieldType, FilterValueType>;

template <typename Of, typename FieldType>
ActualFilter(Field<Of, FieldType>, std::initializer_list<FieldType>, std::string) -> ActualFilter<Of, FieldType, std::initializer_list<FieldType>>;

template<typename Of, typename FieldType, typename FilterValueType>
std::string ActualFilter<Of, FieldType, FilterValueType>::serialize()
{
    return serializeActualFilter(this->op, this->f, this->val);
}

template<typename Of, typename FieldType, typename FilterValueType>
std::string serializeActualFilter(const std::string &op, Field<Of, FieldType> f, FilterValueType val)
{

    auto res = opLookup.find(op);
    if (res == opLookup.end())
    {
        throw "Op not found: " + op;
    }

    return "\"" + f.name + res->second + "\":" + printJsonValue(val);
}

template<typename Of, typename FieldType>
std::string serializeActualFilter(const std::string &op, Field<Of, FieldType> f, std::initializer_list<FieldType> val)
{
    auto res = opLookup.find(op);
    if (res == opLookup.end())
    {
        throw "Op not found: " + op;
    }

    std::string result = "\"" + f.name + res->second + "\":[";

    size_t i = 1;
    for (const auto &el : val)
    {
        result += printJsonValue(el) + (i++ < val.size() ? "," : "]");
    }
    return result;
}
