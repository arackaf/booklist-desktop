#pragma once

#include "booktable.h"
#include "listwidgetitembase.h"

using Data::Books::Book;

class BookListWidgetItem : public QObject, public ListWidgetItem<Book>
{
    Q_OBJECT

public:
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
