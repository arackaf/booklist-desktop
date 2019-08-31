#pragma once

#include <QLabel>

template <typename T>
class ListWidgetItem
{

public:
    ListWidgetItem(){}

    virtual void init() = 0;

    virtual void bind(const T &) = 0;

    virtual QWidget* getWidget() = 0;

    virtual ~ListWidgetItem(){}
};
