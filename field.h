#pragma once

#include<string>
#include "filter.h"

template<typename Of>
struct Filter;

template<typename Of, typename T>
struct ActualFilter;

template <typename Of, typename T>
struct Field
{
    Field(const std::string &val): name(val) { }
    Field(std::string &&val): name(std::move(val)) { }
    std::string name;

    std::shared_ptr<Filter<Of>> in(const std::initializer_list<T> &);
};

template <typename Of, typename T>
struct ArrayField : public Field<Of, T>
{
    using Field<Of, T>::Field;
    std::shared_ptr<Filter<Of>> matches(const std::initializer_list<T> &);
};

template <typename Of, typename T>
std::shared_ptr<Filter<Of>> Field<Of, T>::in(const std::initializer_list<T> &vals)
{
    return std::make_shared<ActualFilter<Of, std::initializer_list<T>>>(ActualFilter<Of, std::initializer_list<T>>{ *this, vals, "in" });
}

template <typename Of, typename T>
std::shared_ptr<Filter<Of>> ArrayField<Of, T>::matches(const std::initializer_list<T> &vals)
{
    return std::make_shared<ActualFilter<Of, std::initializer_list<T>>>(ActualFilter<Of, std::initializer_list<T>>{ *this, vals, "==" });
}

// operator ==

template <typename Of, typename T>
std::shared_ptr<Filter<Of>> operator==(Field<Of, T> f, const T &val)
{
    return val == f;
}

template <typename Of, typename T>
std::shared_ptr<Filter<Of>> operator==(const T &val, const Field<Of, T> &f)
{
    return std::make_shared<ActualFilter<Of, T>>(ActualFilter<Of, T>{ f, val, "==" });
}

template <typename Of, size_t N>
std::shared_ptr<Filter<Of>> operator==(Field<Of, std::string> f, const char (&val) [N])
{
    return val == f;
}

template <typename Of, size_t N>
std::shared_ptr<Filter<Of>> operator==(const char (&val) [N], Field<Of, std::string> f)
{
    return std::make_shared<ActualFilter<Of, std::string>>(ActualFilter<Of, std::string>{ f, val, "==" });
}

//operator !=

template <typename Of, typename T>
std::shared_ptr<Filter<Of>> operator!=(Field<Of, T> f, const T &val)
{
    return val != f;
}

template <typename Of, typename T>
std::shared_ptr<Filter<Of>> operator!=(const T &val, const Field<Of, T> &f)
{
    return std::make_shared<ActualFilter<Of, T>>(ActualFilter<Of, T>{ f, val, "!=" });
}

template <typename Of, size_t N>
std::shared_ptr<Filter<Of>> operator!=(Field<Of, std::string> f, const char (&val) [N])
{
    return val != f;
}

template <typename Of, size_t N>
std::shared_ptr<Filter<Of>> operator!=(const char (&val) [N], Field<Of, std::string> f)
{
    return std::make_shared<ActualFilter<Of, std::string>>(ActualFilter<Of, std::string>{ f, val, "!=" });
}

// operator <

template <typename Of>
std::shared_ptr<Filter<Of>> operator<(Field<Of, int> f, const int &val)
{
    return val < f;
}

template <typename Of>
std::shared_ptr<Filter<Of>> operator<(const int &val, const Field<Of, int> &f)
{
    return std::make_shared<ActualFilter<Of, int>>(ActualFilter<Of, int>{ f, val, "<" });
}

template <typename Of>
std::shared_ptr<Filter<Of>> operator<(Field<Of, double> f, const double &val)
{
    return val < f;
}

template <typename Of>
std::shared_ptr<Filter<Of>> operator<(const double &val, const Field<Of, double> &f)
{
    return std::make_shared<ActualFilter<Of, double>>(ActualFilter<Of, double>{ f, val, "<" });
}
