#pragma once

#include "mongoquerybase.h"
#include "field.h"
#include <json.hpp>

namespace Data {
namespace Books {

struct Books
{
    std::string _id;
    std::string title;
    int pages;
};

void from_json(const nlohmann::json &j, Books &b)
{
    auto _id = j.at("_id");
    b._id = _id.empty() ? std::string{} : _id.get<std::string>();

    auto title = j.at("title");
    b.title = title.empty() ? std::string{} : title.get<std::string>();

    auto pages = j.at("pages");
    b.pages = pages.empty() ? int{} : pages.get<int>();
}

//TODO: create these with a Macro
extern Field<Books, std::string> title;
inline Field<Books, std::string> title = Field<Books, std::string> { "title" };

extern Field<Books, std::string> smallImage;
inline Field<Books, std::string> smallImage = Field<Books, std::string>{ "smallImage" };

extern Field<Books, int> pages;
inline Field<Books, int> pages = Field<Books, int>{ "pages" };

extern Field<Books, int> edition;
inline Field<Books, int> edition = Field<Books, int>{ "edition" };

extern Field<Books, double> weight;
inline Field<Books, double> weight = Field<Books, double>{ "weight" };

extern ArrayField<Books, std::string> authors;
inline ArrayField<Books, std::string> authors = ArrayField<Books, std::string>{ "authors" };

}
}
