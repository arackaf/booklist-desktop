#pragma once

#include <json.hpp>

#include "filter.h"
#include "filterUtils.h"

template <typename Of, typename T>
struct Field;

template <typename Of, typename T>
struct NumericField;

template <typename Of, typename FieldType, typename FilterValueType = FieldType>
struct ActualFilter: public Filter<Of>
{
    ActualFilter(const Field<Of, FieldType> &f, const FilterValueType val, const std::string &op) : val(val), op(op), f(f) {}
    FilterValueType val;
    std::string op;
    Field<Of, FieldType> f;
    void addToSerialization(nlohmann::json &) override;
};

template <typename Of, typename FieldType>
ActualFilter(Field<Of, FieldType>, const std::initializer_list<FieldType> &, std::string) -> ActualFilter<Of, FieldType, std::initializer_list<FieldType>>;

template <typename Of>
ActualFilter(Field<Of, std::string>, const std::initializer_list<const char *> &, std::string) -> ActualFilter<Of, std::string, std::initializer_list<std::string>>;


template<typename Of, typename FieldType, typename FilterValueType>
void ActualFilter<Of, FieldType, FilterValueType>::addToSerialization(nlohmann::json &j)
{
    auto res = opLookup.find(op);
    if (res == opLookup.end())
    {
        throw "Op not found: " + op;
    }

    j[f.name + res->second] = val;
}

