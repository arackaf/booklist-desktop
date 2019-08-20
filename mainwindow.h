#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkReply>
#include <QLabel>

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

    ImageLoader(const std::string &url, const std::string &name, const std::function<void()> &updater) : name(name), url(url), updater(updater) {
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
};

#endif // MAINWINDOW_H
