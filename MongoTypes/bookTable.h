#pragma once

#include "query.h"
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


// ------------------------------ Type Classes -----------------------------------------


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


// ------------------------------ Type Serializations ----------------------------------


#define Field(name, type) obj.name = j.contains(#name) ? j.at(#name).get<type>() : type{};
#define ArrayField(name, type) obj.name = j.contains(#name) ? j.at(#name).get<std::vector<type>>() : std::initializer_list<type>{};

#define SERIALIZE(namespaceName, className, Expansion) namespace namespaceName { \
    extern void from_json(const nlohmann::json &j, className &obj); \
    inline void from_json(const nlohmann::json &j, className &obj) { \
        Expansion \
    } \
}

SERIALIZE(Books, Book, BookList)

#undef Field
#undef ArrayField
    
// ------------------------------- Type Filters -------------------------------------------


#define Field(name, type) extern ScopedField<type>::Type name; \
    inline ScopedField<type>::Type name = ScopedField<type>::Type { #name };

#define ArrayField(name, type) extern ScopedArrayField<type> name; \
    inline ScopedArrayField<type> name = ScopedArrayField<type> { #name };

#define FILTERS(namespaceName, className, Expansion) namespace namespaceName { \
    template <typename T> \
    struct ScopedField { using Type = Field<className, T>; }; \
    template <> \
    struct ScopedField<std::string> { using Type = StringField<className>; }; \
    \
    template <typename T> \
    using ScopedArrayField = ArrayField<className, T>; \
    Expansion \
}

FILTERS(Books, Book, BookList)

#undef Field
#undef ArrayField


//TDOD: properly redefine
namespace Subjects {

class Subjects{};

extern Field<Subjects, std::string> name;
inline Field<Subjects, std::string> name = Field<Subjects, std::string> { "name" };

}

}
