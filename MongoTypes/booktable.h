#pragma once

#include "mongoquerybase.h"
#include "field.h"
#include <json.hpp>

#define BookList \
    Field(_id, std::string) \
    Field(title, std::string) \
    Field(smallImage, std::string) \
    Field(mediumImage, std::string) \
    Field(pages, int) \
    Field(edition, int) \
    Field(weight, double) \
    ArrayField(authors, std::string)

namespace Data {

// ------------------------------ All Types' Classes -----------------------------------------

#define Field(name, type) type name;
#define ArrayField(name, type) std::vector<type> name;

#define CLASS(namespaceName, className, Expansion) namespace namespaceName { \
    struct className { \
        Expansion \
    }; \
}

CLASS(Books, Book, BookList)

#undef Field
#undef ArrayField


//#define Field(name) FilterDeclaration(name, double);
//#define ArrayField(name) ArrayFilterDeclaration(name, std::string);

// ------------------------------ End Types' Classes -----------------------------------------

// ------------------------------ All Type's Serializations ----------------------------------

namespace Books {

void from_json(const nlohmann::json &j, Book &b)
{
    auto _id = j.at("_id");
    b._id = _id.empty() ? std::string{} : _id.get<std::string>();
    
    auto title = j.at("title");
    b.title = title.empty() ? std::string{} : title.get<std::string>();
    
    auto pages = j.at("pages");
    b.pages = pages.empty() ? int{} : pages.get<int>();

    auto authors = j.at("authors");
    b.authors = authors.empty() ? std::initializer_list<std::string>{} : authors.get<std::vector<std::string>>();
}

}

// ------------------------------ End Types' Serializations -------------------------------------
    
// ------------------------------- All Types' Filters -------------------------------------------



#define Field(name, type) extern ScopedField<type> name; \
    inline ScopedField<type> name = ScopedField<type> { #name };

#define ArrayField(name, type) extern ScopedArrayField<type> name; \
    inline ScopedArrayField<type> name = ScopedArrayField<type> { #name };

#define FILTERS(namespaceName, className, Expansion) namespace namespaceName { \
    template <typename T> \
    using ScopedField = Field<className, T>; \
    template <typename T> \
    using ScopedArrayField = ArrayField<className, T>; \
    Expansion \
}

// ---------------------------- for each type ---------------------------------

FILTERS(Books, Book, BookList)

}
