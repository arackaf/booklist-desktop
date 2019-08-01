#pragma once

#include "mongoquerybase.h"
#include "field.h"

namespace Data {
namespace Books {

class Books
{
};

//TODO: create these with a Macro
extern Field<Books, std::string> title;
inline Field<Books, std::string> title = Field<Books, std::string> { "title" };

extern Field<Books, std::string> smallImage;
inline Field<Books, std::string> smallImage = Field<Books, std::string>{ "smallImage" };

extern NumericField<Books, int> pages;
inline NumericField<Books, int> pages = NumericField<Books, int>{ "pages" };

extern NumericField<Books, int> edition;
inline NumericField<Books, int> edition = NumericField<Books, int>{ "edition" };

extern NumericField<Books, double> weight;
inline NumericField<Books, double> weight = NumericField<Books, double>{ "weight" };

extern ArrayField<Books, std::string> authors;
inline ArrayField<Books, std::string> authors = ArrayField<Books, std::string>{ "authors" };

}
}
