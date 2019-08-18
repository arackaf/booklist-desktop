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

    ImageLoader(const std::string &url, QLabel *target) : target(target), url(url) {
        this->loadImage();
    }

    void loadImage();

public slots:
    void fileDownloaded(QNetworkReply *pReply);

private:
    QNetworkAccessManager manager;
    QLabel *target;
    std::string url;
};

#endif // MAINWINDOW_H
