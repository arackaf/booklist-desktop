#pragma once

#include "mongoquerybase.h"
#include "field.h"

namespace Data {
namespace Books {

class Books
{
};

//TODO: create these with a Macro
extern Field<Books> title;
inline Field<Books> title = Field<Books> { "title" };

extern Field<Books> smallImage;
inline Field<Books> smallImage = Field<Books>{ "smallImage" };

extern Field<Books> pages;
inline Field<Books> pages = Field<Books>{ "pages" };

extern Field<Books> edition;
inline Field<Books> edition= Field<Books>{ "edition" };

}
}
