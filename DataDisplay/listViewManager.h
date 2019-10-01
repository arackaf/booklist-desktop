#pragma once

#include "listModel.h"
#include "listWidgetItem.h"

#include <QListView>
#include <QStyledItemDelegate>
#include <QDebug>

#include "listViewItemDelegate.h"

template <typename T, typename U, typename = std::enable_if_t<std::is_base_of_v<ListWidgetItem<T>, U>>>
class ListViewManager {
public:
    ListViewManager(QListView *listView, size_t size) : maxSize(size), listModel(new ListModel<T>{})
    {
        this->listView = listView;
        this->listWidgetPool = std::vector<std::shared_ptr<ListWidgetItem<T>>>(size);

        this->listView->setModel(this->listModel);
        this->listView->setResizeMode(QListView::Adjust);

        this->updateListViewSizing = [this](){ this->_updateListViewSizing(); };

        for (auto i = 0; i < size; i++)
        {
            this->listWidgetPool[i] = std::make_shared<U>(this->updateListViewSizing);
        }
        this->listView->setItemDelegate(new ViewDelegate<T>{ this->listWidgetPool });
    }

    virtual ~ListViewManager() {}

    void setData(const std::vector<T> &results);

private:

    void _updateListViewSizing();

    QListView *listView;
    ListModel<T> *listModel;
    std::vector<std::shared_ptr<ListWidgetItem<T>>> listWidgetPool;
    size_t maxSize;
    std::function<void()> updateListViewSizing;
};

template <typename T, typename U, typename V>
void ListViewManager<T, U, V>::setData(const std::vector<T> &results)
{
    this->listModel->newData(results);
    for (size_t i = 0; i < results.size(); i++)
    {
        auto w = this->listWidgetPool[i];
        w->bind(results[i]);
        this->listView->setIndexWidget(this->listModel->index(i), w->getWidget());
    }
}

template <typename T, typename U, typename V>
void ListViewManager<T, U, V>::_updateListViewSizing()
{
    QSize size = this->listView->viewport()->size();
    size.setHeight(size.height() + 1);
    this->listView->viewport()->resize(size);

    size.setHeight(size.height()-1);
    this->listView->viewport()->resize(size);
}
