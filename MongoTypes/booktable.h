#pragma once

#include "mongoquerybase.h"
#include "field.h"
#include <json.hpp>

namespace Data {
namespace Books {

struct Book
{
    std::string _id;
    std::string title;
    int pages;
};

void from_json(const nlohmann::json &j, Book &b)
{
    auto _id = j.at("_id");
    b._id = _id.empty() ? std::string{} : _id.get<std::string>();

    auto title = j.at("title");
    b.title = title.empty() ? std::string{} : title.get<std::string>();

    auto pages = j.at("pages");
    b.pages = pages.empty() ? int{} : pages.get<int>();
}

//TODO: create these with a Macro
extern Field<Book, std::string> title;
inline Field<Book, std::string> title = Field<Book, std::string> { "title" };

extern Field<Book, std::string> smallImage;
inline Field<Book, std::string> smallImage = Field<Book, std::string>{ "smallImage" };

extern Field<Book, int> pages;
inline Field<Book, int> pages = Field<Book, int>{ "pages" };

extern Field<Book, int> edition;
inline Field<Book, int> edition = Field<Book, int>{ "edition" };

extern Field<Book, double> weight;
inline Field<Book, double> weight = Field<Book, double>{ "weight" };

extern ArrayField<Book, std::string> authors;
inline ArrayField<Book, std::string> authors = ArrayField<Book, std::string>{ "authors" };

}
}
