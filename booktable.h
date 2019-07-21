#pragma once

#include "mongoquerybase.h"
#include "field.h"

namespace Data {
namespace Books {

class Books
{
};

extern Field<Books> title;
Field<Books> title = Field<Books> { "title" };

extern Field<Books> smallImage;
Field<Books> smallImage = Field<Books>{ "smallImage" };

}
}
