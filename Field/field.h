#pragma once

#include<string>
#include "filter.h"
#include "actualFilter.h"

template <typename Of, typename T>
struct Field
{
    Field(const std::string &val): name(val) { }
    Field(std::string &&val): name(std::move(val)) { }
    std::string name;

    std::shared_ptr<Filter<Of>> in(const std::initializer_list<T> &);
    Field as(const std::string &);
};

template <typename Of>
struct StringField : public Field<Of, std::string>
{
    using Field<Of, std::string>::Field;
    std::shared_ptr<Filter<Of>> contains(const std::string &);
};

template <typename Of>
std::shared_ptr<Filter<Of>> StringField<Of>::contains(const std::string &val)
{
    return std::make_shared<ActualFilter<Of, std::string>>(*this, val, "contains");
}


template <typename Of, typename T>
struct ArrayField : public Field<Of, T>
{
    using Field<Of, T>::Field;
    std::shared_ptr<Filter<Of>> matches(const std::initializer_list<T> &);
};

template <typename Of, typename T>
Field<Of, T> Field<Of, T>::as(const std::string &alias)
{
    return Field<Of, T>{ alias };
}

template <typename Of, typename T>
std::shared_ptr<Filter<Of>> Field<Of, T>::in(const std::initializer_list<T> &vals)
{
    return std::make_shared<ActualFilter<Of, T, std::initializer_list<T>>>(*this, vals, "in");
}

template <typename Of, typename T>
std::shared_ptr<Filter<Of>> ArrayField<Of, T>::matches(const std::initializer_list<T> &vals)
{
    return std::make_shared<ActualFilter<Of, T, std::initializer_list<T>>>(*this, vals, "==");
}

//  operator &&

template <typename Of, typename T, typename U>
std::shared_ptr<AndFilter<Of>> operator &&(const std::shared_ptr<ActualFilter<Of, T>> &lhs, const std::shared_ptr<ActualFilter<Of, U>> &rhs)
{
    return std::make_shared<AndFilter<Of>>(AndFilter<Of>{ lhs, rhs });;
}

template <typename Of, typename T>
std::shared_ptr<AndFilter<Of>> operator &&(const std::shared_ptr<ActualFilter<Of, T>> &lhs, const std::shared_ptr<AndFilter<Of>> &rhs)
{
    return rhs && lhs;
}

template <typename Of, typename T>
std::shared_ptr<AndFilter<Of>> operator &&(const std::shared_ptr<AndFilter<Of>> &lhs, const std::shared_ptr<ActualFilter<Of, T>> &rhs)
{
    lhs->filters.push_back(rhs);
    return lhs;
}

template <typename Of, typename T>
std::shared_ptr<AndFilter<Of>> operator &&(const std::shared_ptr<ActualFilter<Of, T>> &lhs, const std::shared_ptr<OrFilter<Of>> &rhs)
{
    return rhs && lhs;
}

template <typename Of, typename T>
std::shared_ptr<AndFilter<Of>> operator &&(const std::shared_ptr<OrFilter<Of>> &lhs, const std::shared_ptr<ActualFilter<Of, T>> &rhs)
{
    return std::make_shared<AndFilter<Of>>(AndFilter<Of>{ lhs, rhs });
}

//------

template <typename Of>
std::shared_ptr<AndFilter<Of>> operator &&(const std::shared_ptr<AndFilter<Of>> &lhs, const std::shared_ptr<AndFilter<Of>> &rhs)
{
    lhs->filters.push_back(rhs);
    return lhs;
}

template <typename Of>
std::shared_ptr<AndFilter<Of>> operator &&(const std::shared_ptr<OrFilter<Of>> &lhs, const std::shared_ptr<OrFilter<Of>> &rhs)
{
    return std::make_shared<AndFilter<Of>>(AndFilter<Of>{ lhs, rhs });
}


// operator ==

template <typename Of, typename T>
std::shared_ptr<ActualFilter<Of, T>> operator==(const Field<Of, T> &f, const T &val)
{
    return val == f;
}

template <typename Of, typename T>
std::shared_ptr<ActualFilter<Of, T>> operator==(const T &val, const Field<Of, T> &f)
{
    return std::make_shared<ActualFilter<Of, T>>(ActualFilter<Of, T>{ f, val, "==" });
}

template <typename Of, size_t N>
std::shared_ptr<ActualFilter<Of, std::string>> operator==(const Field<Of, std::string> &f, const char (&val) [N])
{
    return val == f;
}

template <typename Of, size_t N>
std::shared_ptr<ActualFilter<Of, std::string>> operator==(const char (&val) [N], Field<Of, std::string> f)
{
    return std::make_shared<ActualFilter<Of, std::string>>(ActualFilter<Of, std::string>{ f, val, "==" });
}

//operator !=

template <typename Of, typename T>
std::shared_ptr<ActualFilter<Of, T>> operator!=(const Field<Of, T> &f, const T &val)
{
    return val != f;
}

template <typename Of, typename T>
std::shared_ptr<ActualFilter<Of, T>> operator!=(const T &val, const Field<Of, T> &f)
{
    return std::make_shared<ActualFilter<Of, T>>(ActualFilter<Of, T>{ f, val, "!=" });
}

template <typename Of, size_t N>
std::shared_ptr<ActualFilter<Of, std::string>> operator!=(Field<Of, std::string> f, const char (&val) [N])
{
    return val != f;
}

template <typename Of, size_t N>
std::shared_ptr<ActualFilter<Of, std::string>> operator!=(const char (&val) [N], const Field<Of, std::string> &f)
{
    return std::make_shared<ActualFilter<Of, std::string>>(ActualFilter<Of, std::string>{ f, val, "!=" });
}

// operator <



