#include "bookListWidgetItem.h"

#include <QGridLayout>
#include <QPushButton>
#include <QDebug>
#include <QApplication>

#include <fstream>

std::string imageUrlToFilename(std::string s)
{
    size_t nextLoc;
    while ((nextLoc = s.find("/")) != std::string::npos)
    {
        s = s.substr(nextLoc + 1, s.length() - (nextLoc + 1));
    }

    return s;
}

void BookListWidgetItem::init()
{
    w = new QWidget();
    auto gl = new QGridLayout{};

    //gl->setMargin(0);

    gl->setContentsMargins(10, 10, 0, 10);

    gl->setColumnMinimumWidth(0, 65);
    gl->setColumnStretch(0, 0);

    gl->setColumnMinimumWidth(1, 200);
    gl->setColumnStretch(1, 1);

    l = new QLabel{""};
    imgL = new QImage;
    //imgL->load(QString { url.c_str() });
    //imgL->load(QString{ "/Users/adam.rackis/Documents/booklist-local/smallImages/5d766937b552ee000400800f.jpg" });
    imgL->load(QString{ "/Users/arackis/Documents/git/booklist-desktop/saved_covers/yellow.jpg" });
    l->setPixmap(QPixmap::fromImage(*imgL));
    l->adjustSize();

    const std::function<void()> refresh = [](){};

    gl->addWidget(l, 0, 0, Qt::AlignTop);
    titleLabel = new QLabel { "a" };
    gl->addWidget(titleLabel, 0, 1, Qt::AlignTop);

    gl->setHorizontalSpacing(0);
    gl->setVerticalSpacing(0);

    w->setLayout(gl);

    //ImageLoader *il = new ImageLoader{remote, newFile, refresh, this};

}

void BookListWidgetItem::updateImage(const std::string &newImgPath)
{
    if (this->currentImagePath != newImgPath){
        return;
    }
    QImage *newImg = new QImage;
    newImg->load(QString { newImgPath.c_str() });

    l->setPixmap(QPixmap::fromImage(*newImg));
    l->adjustSize();
    this->w->adjustSize();
    this->itemUpdated();
}

void BookListWidgetItem::bind(const Book &b)
{
    titleLabel->setText(QString::fromStdString(b.title));

    std::string smallImageLocalPath = "/Users/arackis/Documents/git/booklist-desktop/saved_covers/smallImages/" + imageUrlToFilename(b.smallImage);
    this->currentImagePath = smallImageLocalPath;


    if (!FileLoader::fileExists(smallImageLocalPath))
    {
        if (!this->fileLoader)
        {
            this->fileLoader = std::make_shared<FileLoader>();
            connect(this->fileLoader.get(), &FileLoader::doneDownloading, [this](const std::string &str) { this->updateImage(str); });
        }
        this->fileLoader->loadImage(b.smallImage, smallImageLocalPath);
    }
    else
    {
        this->updateImage(smallImageLocalPath);
    }


}

QWidget* BookListWidgetItem::getWidget()
{
    return this->w;
}
