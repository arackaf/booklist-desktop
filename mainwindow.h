#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkReply>
#include <QLabel>
#include <QAbstractListModel>

#include "bookListWidgetItem.h"
#include "bookTable.h"

#include "graphQLLoader.h"
#include "listViewManager.h"


using Data::Books::Book;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private:
    Ui::MainWindow *ui;
    GraphQLLoader<Book> *loader;
    ListViewManager<Book, BookListWidgetItem> *listViewManager;
};


#endif // MAINWINDOW_H
