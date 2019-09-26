#pragma once

#include <vector>
#include <string>
#include <curl/curl.h>
#include <json.hpp>

using json = nlohmann::json;


template <typename T>
class GraphQLLoader {

public:
    GraphQLLoader(const std::function<void(std::vector<T>)> cb) : cb(cb)
    {
        curl = curl_easy_init();
    }
    void load(std::string url);

    ~GraphQLLoader()
    {
        curl_easy_cleanup(curl);
    }


private:
    std::function<void(std::vector<T>)> cb;
    CURL *curl;
    CURLcode res;
    std::string readBuffer;
};

static size_t CurlWriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

template <typename T>
void GraphQLLoader<T>::load(std::string url)
{
    if(curl) {
      curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWriteCallback);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
      res = curl_easy_perform(curl);

      auto j2 = json::parse(readBuffer);

      auto dataMaybe = j2.find("data");
      auto allBooksMaybe = dataMaybe->find("allBooks");
      auto booksMaybe = allBooksMaybe->find("Books");

      auto booksCount = booksMaybe->size();

      std::vector<T> result(booksCount);

      result.assign(booksMaybe->begin(), booksMaybe->end());

      this->cb(std::move(result));
      //return std::move(result);
    }
}





