#include <curl/curl.h>
#include <string>
#include "filter.h"

template <>
std::string serializeVal<std::string>(std::string val)
{
    return "\"" + val + "\"";
}
