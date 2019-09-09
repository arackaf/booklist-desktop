#pragma once

#include "listModel.h"
#include "listWidgetItemBase.h"

#include <QListView>
#include <QStyledItemDelegate>
#include <QDebug>

template <typename T>
struct ViewDelegate : public QStyledItemDelegate
{
    ViewDelegate(std::vector<std::shared_ptr<T>> widgets) : QStyledItemDelegate(nullptr), widgets(widgets) {}

    QSize sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const;

    std::vector<std::shared_ptr<T>> widgets;
};

template <typename T>
QSize ViewDelegate<T>::sizeHint(const QStyleOptionViewItem &, const QModelIndex &index) const
{
    auto height = this->widgets[index.row()]->getWidget()->sizeHint().height();
    qDebug() << "New Height" << height;
    return QSize(0, height); //enter your values here
}

template <typename T, typename U, typename = std::enable_if_t<std::is_base_of_v<ListWidgetItem<T>, U>>>
class ListViewManager {
public:
    ListViewManager(QListView *listView, size_t size) : maxSize(size), listModel(new ListModel<T>{}) {
        this->listView = listView;
        this->listWidgetPool = std::vector<std::shared_ptr<U>>(size);

        this->listView->setModel(this->listModel);

        for (auto i = 0; i < size; i++)
        {
            this->listWidgetPool[i] = std::make_shared<U>();
        }
        this->listView->setItemDelegate(new ViewDelegate<U>{ this->listWidgetPool });
    }

    ~ListViewManager()
    {
        for (auto i = 0; i < maxSize; i++)
        {
            delete this->listWidgetPool[i];
        }
    }

    void setData(const std::vector<T> &results)
    {
        this->listModel->newData(results);
        for (size_t i = 0; i < results.size(); i++)
        {
            auto w = this->listWidgetPool[i];
            w->bind(results[i]);
            this->listView->setIndexWidget(this->listModel->index(i), w->getWidget());
        }
    }

private:

    QListView *listView;
    ListModel<T> *listModel;
    std::vector<std::shared_ptr<U>> listWidgetPool;
    size_t maxSize;
};
