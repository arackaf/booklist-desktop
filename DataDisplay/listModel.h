#pragma once

#include <QAbstractListModel>
#include <QModelIndex>
#include <QDebug>

template <typename T>
class ListModel : public QAbstractListModel
{
private:
    std::vector<T> items;
public:
    ListModel(QObject *parent = nullptr) : QAbstractListModel(parent) { }
    void modelDataChanged(const QModelIndex & = QModelIndex{}, const QModelIndex & = QModelIndex{});
    int rowCount(const QModelIndex & = QModelIndex{}) const override;
    QVariant data(const QModelIndex &index, int = Qt::DisplayRole) const override;

    void newData(const std::vector<T> &);
};

template <typename T>
void ListModel<T>::modelDataChanged(const QModelIndex &start, const QModelIndex &end)
{
    emit dataChanged(QModelIndex(), QModelIndex());
    QModelIndex top = createIndex(0, 0);
    //for now
    QModelIndex bottom = createIndex(rowCount(), 0);
    emit dataChanged(top, bottom);
}

template <typename T>
int ListModel<T>::rowCount(const QModelIndex &) const
{
    return items.size();
}

template <typename T>
void ListModel<T>::newData(const std::vector<T> &newData)
{
    items.assign(newData.begin(), newData.end());
    this->modelDataChanged();
}

template <typename T>
QVariant ListModel<T>::data(const QModelIndex &index, int role) const
{
    return QVariant{};
}
