#pragma once

#include <QMainWindow>
#include <QNetworkReply>
#include <QLabel>
#include <QAbstractListModel>

class FileLoader : public QObject {
    Q_OBJECT

public:

    FileLoader(const std::string &url, const std::string &savePath) : url(url), savePath(savePath) {
        this->loadImage();
    }

    void loadImage();

public slots:
    void fileDownloaded(QNetworkReply *pReply);

signals:
    void doneDownloading(const std::string &);

private:
    QNetworkAccessManager manager;
    std::string url;
    std::string savePath;
};
