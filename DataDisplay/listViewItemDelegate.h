#pragma once

#include <QStyledItemDelegate>

#include "listWidgetItem.h"

template <typename T>
struct ViewDelegate : public QStyledItemDelegate
{
    ViewDelegate(std::vector<std::shared_ptr<ListWidgetItem<T>>> widgets) : QStyledItemDelegate(nullptr), widgets(widgets) {}

    QSize sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const;

    std::vector<std::shared_ptr<ListWidgetItem<T>>> widgets;
};

template <typename T>
QSize ViewDelegate<T>::sizeHint(const QStyleOptionViewItem &, const QModelIndex &index) const
{
    auto height = this->widgets[index.row()]->getWidget()->sizeHint().height();
    return QSize(0, height);
}
