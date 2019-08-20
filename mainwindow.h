#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkReply>
#include <QLabel>

class ListWidgetItem : public QObject
{
    Q_OBJECT

public:
    ListWidgetItem(const std::string &url, const std::string &remote, const std::string &newFile) : url(url), remote(remote), newFile(newFile)
    {
        this->init();
    }

    void init();

    QWidget* getWidget(){ return this->w; }

public slots:
    void updateImage(QNetworkReply* pReply);

private:
    QWidget* w;
    QLabel *l;
    QImage *imgL;
    std::string url;
    std::string remote;
    std::string newFile;
};

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

    ImageLoader(const std::string &url, const std::string &name, const std::function<void()> &updater, ListWidgetItem *lwi) : name(name), url(url), updater(updater), lwi(lwi) {
        this->loadImage();
    }

    void loadImage();

public slots:
    void fileDownloaded(QNetworkReply *pReply);

private:
    QNetworkAccessManager manager;
    std::string name;
    std::string url;
    std::function<void()> updater;
    ListWidgetItem *lwi;
};

#endif // MAINWINDOW_H
