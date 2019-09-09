#include <string>
#include "filterUtils.h"

template <>
std::string printJsonValue<std::string>(std::string val)
{
    return "\"" + val + "\"";
}

const std::map<std::string, std::string> opLookup = std::map<std::string, std::string>
{
    { "==", "" },
    { "<", "_lt" },
    { "<=", "_lte" },
    { ">", "_gt" },
    { ">=", "_gte" },
    { "in", "_in" }
};
