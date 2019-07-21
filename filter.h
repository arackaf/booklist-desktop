#pragma once

#include <iostream>
#include "field.h"

template<typename Of, typename T>
struct Filter
{
    Field<Of> f;
    T val;
    std::string op;
};

//template <typename T>
//Filter<T> Field::operator==(const T &val)
//{
//    return Filter<T>{ *this, val, "==" };
//}


//template <std::size_t N>
//Filter<std::string> Field::operator==(const char (&val) [N])
//{
//   return Filter<std::string>{ *this, std::string(val), "==" };
//}

//struct Query
//{
//    Query(): query(""){}
//    std::string query;

//    template <typename T>
//    void MakeFilter(Filter<T> f);

//    template <typename T, typename... Ts>
//    void MakeFilter(Filter<T>, Ts... rest);
//};

//template <typename T>
//void Query::MakeFilter(Filter<T> filter)
//{
//    std::cout<<filter.f.name<<filter.op<<filter.val<<std::endl;
//    query = query + filter.f.name;
//}

//template <typename T, typename... Ts>
//void Query::MakeFilter(Filter<T> f, Ts... rest)
//{
//    MakeFilter(f);
//    MakeFilter(rest...);
//}

//struct PersonTable {
//    template <typename T, typename... Ts>
//    Query RunFilter(Filter<T> first, Ts... rest);

//    Field firstName{ "firstName" };
//    Field lastName{ "lastName" };
//    Field address{ "address" };
//    Field age{ "age" };
//};

//template <typename T, typename... Ts>
//Query PersonTable::RunFilter(Filter<T> first, Ts... rest)
//{
//    Query res;
//    res.MakeFilter(first, rest...);
//    return res;
//}
