#pragma once

#include<string>
#include "filter.h"

template <typename Of>
struct Field
{
    Field(const std::string &val): name(val) { }
    Field(std::string &&val): name(std::move(val)) { }
    std::string name;
};
