#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "filter.h"
#include "query.h"
#include <curl/curl.h>
#include <json.hpp>
using json = nlohmann::json;

#include "query.h"
#include "mongoQueryBase.h"
#include "bookTable.h"
#include "listModel.h"
#include "graphQLLoader.h"
#include "listViewManager.h"

#include <QAbstractListModel>
#include <QStandardItemModel>
#include <QStringList>
#include <QVBoxLayout>
#include <QStyledItemDelegate>
#include <QPushButton>
#include <QLabel>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QObject>
#include <fstream>
#include <QDebug>

using Data::Books::Book;


struct BookViewDelegate : public QStyledItemDelegate
{
    BookViewDelegate(int height, QObject* parent = nullptr) : QStyledItemDelegate(parent), height(height) {}

    int height;

    QSize sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const
    {
        return QSize(0,  height); //enter your values here
    }
};


static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    std::string _url3 = "https://mylibrary.io/graphql-public?query=%7B%0A%20%20allBooks(PAGE%3A1%2C%20PAGE_SIZE%3A%2050%2C%20userId%3A%20%22HelloWorld%22)%7B%0A%20%20%20%20Books%7B%0A%20%20%20%20%20%20title%0A%20%20%20%20%20%20authors%0A%20%20%20%20%20%20_id%0A%20%20%20%20%20%20ean%0A%20%20%20%20%20%20smallImage%0A%20%20%20%20%20%20mediumImage%0A%20%20%20%20%20%20userId%0A%20%20%20%20%7D%0A%20%20%7D%0A%7D";
    std::string url3 = "https://mylibrary.io/graphql-public?query=%7B%0A%20%20allBooks(PAGE%3A1%2C%20PAGE_SIZE%3A%2050%2C%20userId%3A%20%22573d1b97120426ef0078aa92%22%2C%20SORT%3A%7Btitle%3A%201%7D)%7B%0A%20%20%20%20Books%7B%0A%20%20%20%20%20%20title%0A%20%20%20%20%20%20authors%0A%20%20%20%20%20%20_id%0A%20%20%20%20%20%20ean%0A%20%20%20%20%20%20smallImage%0A%20%20%20%20%20%20mediumImage%0A%20%20%20%20%20%20userId%0A%20%20%20%20%7D%0A%20%20%7D%0A%7D";

    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
      curl_easy_setopt(curl, CURLOPT_URL, url3.c_str());
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
      res = curl_easy_perform(curl);
      curl_easy_cleanup(curl);

      //std::string junk = readBuffer.c_str();
      auto data = "Hello World";
      std::string encoded = curl_easy_escape(curl, "hello world", 11);


      //ui->textEdit->setPlainText(QString { readBuffer.c_str() });

      auto j2 = json::parse(readBuffer);

      auto dataMaybe = j2.find("data");
      auto allBooksMaybe = dataMaybe->find("allBooks");
      auto BooksMaybe = allBooksMaybe->find("Books");

      auto firstBook = BooksMaybe->at(0);

      auto titleMaybe = firstBook.find("title");
      auto titleMaybe2 = firstBook["title"].get<std::string>();
      auto pleaseDontCrash = firstBook["notHere"];

      auto titleprotect = firstBook["title"].empty();
      auto crashProtect = pleaseDontCrash.empty();

      auto booksCount = BooksMaybe->size();
      Data::Books::Book b = firstBook;


      size_t booksLength = BooksMaybe->size();

      auto x = j2.begin();

      auto y = *x;

      auto z = y.begin();
      auto zz = *z;

      auto BooksBegin = zz.begin();
      auto Books = *BooksBegin;

      auto BooksArrayBegin = Books.begin();
      auto BooksArr = *BooksArrayBegin;

      for (auto el = BooksArr.begin(); el != BooksArr.end(); el++)
      {
          auto X = *el;
          int k = 12;
      }

      int i = 12;
      i++;
    }

    using namespace Data::Books;

    auto q0 = makeFilter<Book>();

    auto q = makeFilter(
//        (weight < 10 || pages < 500) && (pages < 100 || weight < 90),

        title == "Hello World",

        //Data::Subjects::name == "junk",

        weight < 50 || weight < 100 || weight < 100,
        weight < 50 || weight < 100 || weight < 110 || weight < 120,
        weight < 10 || (weight < 50 || weight < 100),
        (weight < 10 || weight < 50) || weight < 100,
        (weight < 10 || weight < 50) || (weight < 100 || weight < 110),

        weight < 50 && weight < 100,
        weight < 10 && (weight < 50 && weight < 100),
        (weight < 10 && weight < 50) && weight < 100,

        weight < 50 || (weight < 100 && weight < 200),
        weight < 50 || weight < 60 || (weight < 100 && weight < 200),
        (weight < 50 || weight < 60) || (weight < 100 && weight < 200),
        (weight < 50 || weight < 100) && weight < 200,

//        title == "Hello World",
//        "World Hello" == smallImage,
        title.in({ "title1", "title2" }) || weight < 50,
//        pages == 20,
//        20 == pages,

//        pages < 20,
//        20 < pages,
//        19 == edition,

//        weight < 9.5,
//        9.5 < weight,
//        weight < 9,   //coerce to double!
//        9 < weight,   //coerce to double!

//        authors.matches({ "author 1", "author 2" }),

//        title.as("titleAlias") == "Hello World",
//        "Hello World" == title.as("titleAlias2"),

        weight < 10 || weight < 50,
        //pages == 20,
        pages < 20,
        weight < 10 || (weight < 50 || weight < 100),
        weight < 10 || weight < 50 || weight < 100
    );

    //std::shared_ptr<Filter<Books>> x = weight < 10 || weight < 50;
    std::shared_ptr<ActualFilter<Book, double>> x = weight < 10;
    std::shared_ptr<ActualFilter<Book, double>> y = weight < 10;



    //Query<Books> qb{ pages == 20, FilterList<Books>{ weight < 10, weight < 50 } };
    //Query<Books> qb{ pages == 20, std::make_shared<OrFilter<Books>>(OrFilter<Books>{ weight < 10, weight < 50 }) };

    int i = 1;
    std::string message = "";
    for (const auto &el : q.filters)
    {
        message += std::to_string(i++) + ": " + el->serialize() + "\n\n";
    }

    ui->textEdit->setPlainText(QString { (message + "\n\n" + q.serialize()).c_str() });



    ListModel<Book> *model = new ListModel<Book>(nullptr);

    GraphQLLoader<Book> loader { };

    std::vector<Book> results = loader.load(url3);

    ListViewManager<Book, BookListWidgetItem> *listViewManager = new ListViewManager<Book, BookListWidgetItem>{ ui->listView, 50 };

    listViewManager->setData(results);

    //auto w = getListItemWidget("https://images-na.ssl-images-amazon.com/images/I/51QjQQuYcmL._SL75_.jpg");
    //std::function<void(QModelIndex, QModelIndex)> updater = [model](QModelIndex l, QModelIndex r) { model->modelDataChanged(l, r); };

    //auto w = getListItemWidget("/Users/adam.rackis/Desktop/covers/a.jpg", "https://my-library-cover-uploads.s3.amazonaws.com/bookCovers/573d1b97120426ef0078aa92/converted-cover-file-9c0e31fd-cf54-41b0-bc4b-2cc0f0badfeb.jpg", "/Users/adam.rackis/Desktop/SAVED_covers/NEW_A.jpg", updater, model);
    //ui->listView->setItemDelegate(new BookViewDelegate(100, this));

//    auto w = new BookListWidgetItem{ "/Users/adam.rackis/Desktop/covers/a.jpg", "https://my-library-cover-uploads.s3.amazonaws.com/bookCovers/573d1b97120426ef0078aa92/converted-cover-file-9c0e31fd-cf54-41b0-bc4b-2cc0f0badfeb.jpg", "/Users/adam.rackis/Desktop/SAVED_covers/NEW_A.jpg" };
//    ui->listView->setIndexWidget(model->index(0), w->getWidget());

//    auto w2 = new BookListWidgetItem{ "/Users/adam.rackis/Desktop/covers/b.jpg", "https://my-library-cover-uploads.s3.amazonaws.com/bookCovers/573d1b97120426ef0078aa92/converted-cover-file-59af95b4-32b0-41f7-8b7e-1bc02ae221b2.jpg", "/Users/adam.rackis/Desktop/SAVED_covers/NEW_B.jpg" };
//    ui->listView->setIndexWidget(model->index(1), w2->getWidget());

    //ui->listView->setItemDelegate(new BookViewDelegate(100, this));
    //ui->listView->setFixedSize(w->size());

    //ui->listView->setItemDelegate(new BookViewDelegate(95, this));

    ui->tableWidget->insertRow(0);
    ui->tableWidget->insertRow(1);
    ui->tableWidget->insertRow(2);

    ui->tableWidget->insertColumn(0);
    ui->tableWidget->insertColumn(1);
    ui->tableWidget->insertColumn(2);
    ui->tableWidget->insertColumn(3);

    //ui->listView->setItemDelegate(new BookViewDelegate(100, this));

    //model->update();
}



MainWindow::~MainWindow()
{
    delete ui;
}
