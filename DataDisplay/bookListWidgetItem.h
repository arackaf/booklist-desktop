#pragma once

#include "bookTable.h"
#include "listWidgetItemBase.h"

using Data::Books::Book;

class BookListWidgetItem : public QObject, public ListWidgetItem<Book>
{
    Q_OBJECT

public:
    BookListWidgetItem() : BookListWidgetItem("/Users/adam.rackis/Desktop/covers/a.jpg", "https://my-library-cover-uploads.s3.amazonaws.com/bookCovers/573d1b97120426ef0078aa92/converted-cover-file-9c0e31fd-cf54-41b0-bc4b-2cc0f0badfeb.jpg", "/Users/adam.rackis/Desktop/SAVED_covers/NEW_A.jpg") {}

    BookListWidgetItem(const std::string &url, const std::string &remote, const std::string &newFile) : url(url), remote(remote), newFile(newFile)
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
    std::string url;
    std::string remote;
    std::string newFile;
};
