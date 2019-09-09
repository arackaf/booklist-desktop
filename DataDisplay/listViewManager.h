#pragma once

#include "listModel.h"
#include "listWidgetItemBase.h"

#include <QListView>

template <typename T, typename U, typename = std::enable_if_t<std::is_base_of_v<ListWidgetItem<T>, U>>>
class ListViewManager {
public:
    ListViewManager(QListView *listView, size_t size) : maxSize(size), listModel(new ListModel<T>{}) {
        this->listView = listView;
        this->listWidgetPool = std::vector<U*>(size);

        this->listView->setModel(this->listModel);

        for (auto i = 0; i < size; i++)
        {
            this->listWidgetPool[i] = new U{};
        }
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
    std::vector<U*> listWidgetPool;
    size_t maxSize;
};
