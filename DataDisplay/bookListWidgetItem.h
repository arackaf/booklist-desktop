#pragma once

#include "bookTable.h"
#include "listWidgetItem.h"
#include "fileLoader.h"

using Data::Books::Book;

std::string imageUrlToFilename(std::string);

class BookListWidgetItem : public QObject, public ListWidgetItem<Book>
{
    Q_OBJECT

public:
    BookListWidgetItem(std::function<void()> itemUpdated) : ListWidgetItem<Book>(itemUpdated)
    {
        this->init();
    }

    virtual void init() override;

    virtual void bind(const Book &) override;

    virtual QWidget* getWidget() override;

    virtual ~BookListWidgetItem() override{}

public slots:
    void updateImage(const std::string &);

private:
    QWidget* w;
    QLabel *l;
    QImage *imgL;
    QLabel *titleLabel;
    std::shared_ptr<FileLoader> fileLoader;
    std::string currentImagePath;
};
