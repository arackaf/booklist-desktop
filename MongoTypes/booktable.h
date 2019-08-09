#pragma once

#include "mongoquerybase.h"
#include "field.h"
#include <json.hpp>


#define BookList \
    Field(title, std::string) \
    Field(smallImage, std::string) \
    Field(mediumImage, std::string) \
    Field(pages, int) \
    Field(edition, int) \
    Field(weight, double) \
    ArrayField(authors, std::string)

namespace Data {

// ------------------------------ All Types' Classes -----------------------------------------


namespace Books {
    struct Books {
        std::string _id;
        std::string title;
        int pages;
    };
    
    void from_json(const nlohmann::json &j, Books &b) {
        auto _id = j.at("_id");
        b._id = _id.empty() ? std::string{} : _id.get<std::string>();
        
        auto title = j.at("title");
        b.title = title.empty() ? std::string{} : title.get<std::string>();
        
        auto pages = j.at("pages");
        b.pages = pages.empty() ? int{} : pages.get<int>();
    }
}


//#define Field(name) FilterDeclaration(name, double);
//#define ArrayField(name) ArrayFilterDeclaration(name, std::string);

// ------------------------------ End Types' Classes -----------------------------------------



#define FilterDeclaration(name, type) extern FieldOf(type) name; \
    inline FieldOf(type) name = FieldOf(type) { "name" };
#define ArrayFilterDeclaration(name, type) extern ArrayFieldOf(type) name; \
    inline ArrayFieldOf(type) name = ArrayFieldOf(type) { "name" };

#define Field(name, type) FilterDeclaration(name, type);
#define ArrayField(name, type) ArrayFilterDeclaration(name, type);

// ------------------------------- All Types' Filters ----------------------------------------

#define FieldOf(type) Field<Books, type>
#define ArrayFieldOf(type) ArrayField<Books, type>

namespace Books {
BookList
}
}
