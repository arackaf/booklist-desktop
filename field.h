#pragma once

#include<string>
#include "filter.h"

template<typename Of>
struct Filter;

template<typename Of>
struct FilterList;

template<typename Of, typename T>
struct ActualFilter;

template <typename Of, typename T>
struct Field
{
    Field(const std::string &val): name(val) { }
    Field(std::string &&val): name(std::move(val)) { }
    std::string name;

    std::shared_ptr<Filter<Of>> in(const std::initializer_list<T> &);
    Field as(const std::string &);
};

template <typename Of, typename T>
struct NumericField : public Field<Of, T> {};

template <typename Of>
struct NumericField<Of, int> : public Field<Of, int>
{
    using Field<Of, int>::Field;
};

template <typename Of>
struct NumericField<Of, double> : public Field<Of, double>
{
    using Field<Of, double>::Field;
};

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
    return std::make_shared<ActualFilter<Of, std::initializer_list<T>>>(ActualFilter<Of, std::initializer_list<T>>{ *this, vals, "in" });
}

template <typename Of, typename T>
std::shared_ptr<Filter<Of>> ArrayField<Of, T>::matches(const std::initializer_list<T> &vals)
{
    return std::make_shared<ActualFilter<Of, std::initializer_list<T>>>(ActualFilter<Of, std::initializer_list<T>>{ *this, vals, "==" });
}

//operator ||

template <typename Of>
std::shared_ptr<Filter<Of>> operator ||(const std::shared_ptr<Filter<Of>> &lhs, const std::shared_ptr<Filter<Of>> &rhs)
{
    return std::make_shared<FilterList<Of>>(FilterList<Of>{ lhs, rhs });
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

template <typename Of, typename T, typename U>
std::shared_ptr<Filter<Of>> operator<(NumericField<Of, T> f, const U &val)
{
    return val < f;
}

template <typename Of, typename T, typename U>
std::shared_ptr<Filter<Of>> operator<(const U &val, const NumericField<Of, T> &f)
{
    using AdjustedType = std::common_type_t<T, U>;
    using FilterType = ActualFilter<Of, AdjustedType>;

    return std::make_shared<FilterType>(FilterType{ f, static_cast<AdjustedType>(val), "<" });
}

