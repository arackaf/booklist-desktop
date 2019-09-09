#include "bookListWidgetItem.h"

#include <QGridLayout>
#include <QPushButton>
#include <QDebug>
#include <QApplication>

void BookListWidgetItem::init()
{
    w = new QWidget();
    auto gl = new QGridLayout{};

    gl->setMargin(0);

    gl->setContentsMargins(10, 10, 0, 10);

    gl->setColumnMinimumWidth(0, 65);
    gl->setColumnStretch(0, 0);

    gl->setColumnMinimumWidth(1, 200);
    gl->setColumnStretch(1, 1);

    l = new QLabel{""};
    imgL = new QImage;
    //imgL->load(QString { url.c_str() });
    //imgL->load(QString{ "/Users/adam.rackis/Documents/booklist-local/smallImages/5d766937b552ee000400800f.jpg" });
    imgL->load(QString{ "/Users/adam.rackis/Desktop/SAVED_covers/yellow.png" });
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

    std::string newPath = "/Users/adam.rackis/Documents/booklist-local/smallImages/" + b._id + ".jpg";

    this->fileLoader = new FileLoader
    {
        b.smallImage,
        newPath
    };

    connect(this->fileLoader, SIGNAL (doneDownloading(const std::string &)), this, SLOT(updateImage(const std::string &)));
}

QWidget* BookListWidgetItem::getWidget()
{
    return this->w;
}
