#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkReply>
#include <QLabel>
#include <QAbstractListModel>

#include "booklistwidgetitem.h"
#include "booktable.h"


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
};

class ImageLoader : public QObject {
    Q_OBJECT

public:

    ImageLoader(const std::string &url, const std::string &name, const std::function<void()> &updater, BookListWidgetItem *lwi) : name(name), url(url), updater(updater), lwi(lwi) {
        this->loadImage();
    }

    void loadImage();

public slots:
    void fileDownloaded(QNetworkReply *pReply);

signals:
    void doneDownloading(const std::string &);

private:
    QNetworkAccessManager manager;
    std::string name;
    std::string url;
    std::function<void()> updater;
    BookListWidgetItem *lwi;
};

#endif // MAINWINDOW_H
