#pragma once

#include <string>
#include <map>

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
