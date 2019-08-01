#include "query.h"
#include <string>

template<>
std::string encode<std::string>(std::string val)
{
    static CURL *curl = curl_easy_init();

    return curl_easy_escape(curl, val.c_str(), val.length());
}
