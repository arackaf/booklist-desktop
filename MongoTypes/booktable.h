#pragma once

#include "mongoquerybase.h"
#include "field.h"

namespace Data {
namespace Books {

class Books {};

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

namespace Subjects {

class Subjects{};

extern Field<Subjects, std::string> name;
inline Field<Subjects, std::string> name = Field<Subjects, std::string> { "name" };

}


}
