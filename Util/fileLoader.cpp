#include <fstream>
#include "fileLoader.h"

void FileLoader::fileDownloaded(QNetworkReply* pReply) {
    QByteArray bts = pReply->readAll();
    std::ofstream file(this->savePath, std::ios::binary);
    file.write(bts.data(), bts.size());
    file.close();

    pReply->deleteLater();

    emit this->doneDownloading(savePath);
}


void FileLoader::loadImage()
{
    QNetworkReply *reply = manager.get(QNetworkRequest(QUrl(url.c_str())));
    connect(&manager, SIGNAL (finished(QNetworkReply*)), this, SLOT(fileDownloaded(QNetworkReply*)));
}
