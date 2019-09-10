#pragma once

#include <QMainWindow>
#include <QNetworkReply>
#include <QLabel>
#include <QAbstractListModel>

class FileLoader : public QObject {
    Q_OBJECT

public:
    static bool fileExists(const std::string &path);
    FileLoader() { }

    void loadImage(const std::string &url, std::string);

signals:
    void doneDownloading(const std::string &);

private slots:
    void fileDownloaded(QNetworkReply *pReply, const std::string &savePath);

private:
    QNetworkAccessManager manager;
};
