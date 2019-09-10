#include <fstream>
#include "fileLoader.h"

#include <QDebug>

void FileLoader::fileDownloaded(QNetworkReply* pReply, const std::string &savePath) {
    QByteArray bts = pReply->readAll();

    std::ofstream file(savePath, std::ios::binary);
    file.write(bts.data(), bts.size());
    file.close();

    pReply->deleteLater();

    emit this->doneDownloading(savePath);
}


void FileLoader::loadImage(const std::string &url, std::string savePath)
{
    QNetworkReply *reply = manager.get(QNetworkRequest(QUrl(url.c_str())));
    QObject::connect(&manager, &QNetworkAccessManager::finished, [this, savePath](QNetworkReply* pReply) { this->fileDownloaded(pReply, savePath); });
}
