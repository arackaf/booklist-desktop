#include "booklistwidgetitem.h"

#include <QGridLayout>
#include <QPushButton>
#include <QDebug>

void BookListWidgetItem::init()
{
    w = new QWidget();
    auto gl = new QGridLayout{};

    gl->setMargin(0);

    gl->setContentsMargins(10, 10, 0, 0);

    gl->setColumnMinimumWidth(0, 65);
    gl->setColumnStretch(0, 0);

    gl->setColumnMinimumWidth(1, 200);
    gl->setColumnStretch(1, 1);

    l = new QLabel{""};
    imgL = new QImage;
    imgL->load(QString { url.c_str() });
    l->setPixmap(QPixmap::fromImage(*imgL));
    l->adjustSize();

    const std::function<void()> refresh = [](){};

    gl->addWidget(l, 0, 0, Qt::AlignTop);
    gl->addWidget(new QPushButton{"Heyooooo"}, 0, 1, Qt::AlignTop);

    gl->setHorizontalSpacing(0);
    gl->setVerticalSpacing(0);

    w->setLayout(gl);

    //ImageLoader *il = new ImageLoader{remote, newFile, refresh, this};

    qDebug() << w->sizeHint().height();

}

void BookListWidgetItem::updateImage(const std::string &newImgPath)
{
    QImage *newImg = new QImage;
    newImg->load(QString { newImgPath.c_str() });

    l->setPixmap(QPixmap::fromImage(*newImg));
    l->adjustSize();

    qDebug() << "UPDATED";
    qDebug() << w->sizeHint().height();
}

void BookListWidgetItem::bind(const Book &b)
{

}

QWidget* BookListWidgetItem::getWidget()
{
    return this->w;
}
