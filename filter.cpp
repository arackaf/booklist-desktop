#include <curl/curl.h>
#include <string>
#include "filter.h"

template<>
std::string encode<std::string>(std::string val)
{
    static CURL *curl = curl_easy_init();

    return curl_easy_escape(curl, val.c_str(), val.length());
}

template <>
std::string serializeVal<std::string>(std::string val)
{
    return val;
}
