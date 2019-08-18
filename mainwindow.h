#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkReply>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void fileDownloaded(QNetworkReply* pReply);

public:
    explicit MainWindow(QWidget *parent = nullptr);
    QNetworkAccessManager manager;

    void getImage();
    QImage img;

    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
