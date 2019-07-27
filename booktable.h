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

extern Field<Books, int> pages;
inline Field<Books, int> pages = Field<Books, int>{ "pages" };

extern Field<Books, int> edition;
inline Field<Books, int> edition= Field<Books, int>{ "edition" };

}
}
