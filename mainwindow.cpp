#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "filter.h"
#include "query.h"
#include <curl/curl.h>
#include <json.hpp>
using json = nlohmann::json;

#include "query.h"
#include "mongoquerybase.h"
#include "booktable.h"

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

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

using Data::Books::Book;

class ListModel : public QAbstractListModel
{
private:
    std::vector<Book> books;
    int count = 3;
public:
    ListModel(QObject *parent) : QAbstractListModel(parent), books({ Book{}, Book{}, Book{} }) {}
    int rowCount(const QModelIndex &) const override;

    QVariant data(const QModelIndex &index, int = Qt::DisplayRole) const override;

    void update();
};

void ListModel::update()
{
    count = 8;
    emit dataChanged(QModelIndex(), QModelIndex());

//    QModelIndex top = createIndex(0, 0);

//    QModelIndex bottom = createIndex(2, 0);

//    emit dataChanged(top, bottom);
}

int ListModel::rowCount(const QModelIndex &) const
{
    return count;
}

QVariant ListModel::data(const QModelIndex &index, int role) const
{
    return QVariant{};
    //return books[index.row()];
    if (role == Qt::DisplayRole ) {
        return QString{ "Hello " };
    }
    return QVariant{};
}

struct BookViewDelegate : public QStyledItemDelegate
{
    BookViewDelegate(int height, QObject* parent = nullptr) : QStyledItemDelegate(parent), height(height) {}

    int height;

    QSize sizeHint(const QStyleOptionViewItem &, const QModelIndex &i) const
    {
        return QSize(0,  i.row() == 0 ? 87 : 89 /*height*/); //enter your values here
    }
};




void ImageLoader::fileDownloaded(QNetworkReply* pReply) {
    QByteArray bts = pReply->readAll();

    std::ofstream file(this->name, std::ios::binary);
    file.write(bts.data(), bts.size());
    file.close();

    //QImage* img = new QImage{};
    //img->loadFromData(bts);

    //this->target->setPixmap(QPixmap::fromImage(*img));
    //this->target->adjustSize();

    pReply->deleteLater();
}


void ImageLoader::loadImage()
{
    QNetworkReply *reply = manager.get(QNetworkRequest(QUrl(url.c_str())));
    connect(&manager, SIGNAL (finished(QNetworkReply*)), this, SLOT(fileDownloaded(QNetworkReply*)));
}


QWidget* getListItemWidget(const std::string &url, const std::string &remote, const std::string name)
{
    auto w = new QWidget();
    auto gl = new QGridLayout{};

    gl->setMargin(0);

    gl->setContentsMargins(10, 10, 0, 0);

    gl->setColumnMinimumWidth(0, 65);
    gl->setColumnStretch(0, 0);

    gl->setColumnMinimumWidth(1, 200);
    gl->setColumnStretch(1, 1);

    QLabel *l = new QLabel{""};
    ImageLoader *il = new ImageLoader{remote, l, name};
    QImage *imgL = new QImage;
    imgL->load(QString { url.c_str() });

    l->setPixmap(QPixmap::fromImage(*imgL));
    l->adjustSize();


    //QWidget *lholder = new QWidget;
    //QHBoxLayout *lboxHolder = new QHBoxLayout;
    //lholder->setFixedHeight(85);
    //lholder->setFixedWidth(65);

    //lholder->setMinimumHeight(95);
    //lholder->setLayout(lboxHolder);
    //lboxHolder->addWidget(l);


    gl->addWidget(l, 0, 0, Qt::AlignTop);
    gl->addWidget(new QPushButton{"Heyooooo"}, 0, 1, Qt::AlignTop);

    gl->setHorizontalSpacing(0);
    gl->setVerticalSpacing(0);

    w->setLayout(gl);

    qDebug() << gl->sizeHint().height();
    qDebug() << w->sizeHint().height();
    qDebug() << l->margin() << " ";

    //lboxHolder->setContentsMargins(10, 5, 5, 5);


    return w;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    std::string url = "https://mylibrary.io/graphql-public?query=%7B%0A%20%20allBooks%7B%0A%20%20%20%20Books%7B%0A%20%20%20%20%20%20title%0A%20%20%20%20%20%20authors%0A%20%20%20%20%7D%0A%20%20%7D%0A%7D";
    std::string url2 = "https://mylibrary.io/graphql-public?query=%7B%0A%20%20allBooks(PAGE%3A1%2C%20PAGE_SIZE%3A%2050)%7B%0A%20%20%20%20Books%7B%0A%20%20%20%20%20%20title%0A%20%20%20%20%20%20authors%0A%20%20%20%20%7D%0A%20%20%7D%0A%7D";
    std::string url3 = "https://mylibrary.io/graphql-public?query=%7B%0A%20%20allBooks(PAGE%3A1%2C%20PAGE_SIZE%3A%205)%7B%0A%20%20%20%20Books%7B%0A%20%20%20%20%20%20title%0A%20%20%20%20%20%20authors%0A%20%20%20%20%20%20_id%0A%20%20%20%20%20%20ean%0A%20%20%20%20%20%20smallImage%0A%20%20%20%20%20%20mediumImage%0A%20%20%20%20%20%20userId%0A%20%20%20%20%7D%0A%20%20%7D%0A%7D";
    std::string url4 = "https://mylibrary.io/graphql-public?query=%7B%0A%20%20allBooks%7B%0A%20%20%20%20Books%7B%0A%20%20%20%20%20%20title%0A%20%20%20%20%20%20authors%0A%20%20%20%20%20%20_id%0A%20%20%20%20%20%20ean%0A%20%20%20%20%20%20smallImage%0A%20%20%20%20%20%20pages%0A%20%20%20%20%20%20mediumImage%0A%20%20%20%20%20%20userId%0A%20%20%20%20%7D%0A%20%20%7D%0A%7D";
    //auto res = system(command.c_str());

    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
      curl_easy_setopt(curl, CURLOPT_URL, url4.c_str());
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

    ListModel *model = new ListModel(nullptr);

    ui->listView->setModel(model);


    //auto w = getListItemWidget("https://images-na.ssl-images-amazon.com/images/I/51QjQQuYcmL._SL75_.jpg");
    auto w = getListItemWidget("/Users/adam.rackis/Desktop/covers/a.jpg", "https://my-library-cover-uploads.s3.amazonaws.com/bookCovers/573d1b97120426ef0078aa92/converted-cover-file-9c0e31fd-cf54-41b0-bc4b-2cc0f0badfeb.jpg", "/Users/adam.rackis/Desktop/SAVED_covers/NEW_A.jpg");
    ui->listView->setIndexWidget(model->index(0), w);
    //ui->listView->setItemDelegate(new BookViewDelegate(100, this));

    auto w2 = getListItemWidget("/Users/adam.rackis/Desktop/covers/b.jpg", "https://my-library-cover-uploads.s3.amazonaws.com/bookCovers/573d1b97120426ef0078aa92/converted-cover-file-59af95b4-32b0-41f7-8b7e-1bc02ae221b2.jpg", "/Users/adam.rackis/Desktop/SAVED_covers/NEW_B.jpg");
    ui->listView->setIndexWidget(model->index(1), w2);
    //ui->listView->setItemDelegate(new BookViewDelegate(100, this));
    //ui->listView->setFixedSize(w->size());

    ui->listView->setItemDelegate(new BookViewDelegate(95, this));

    ui->tableWidget->insertRow(0);
    ui->tableWidget->insertRow(1);
    ui->tableWidget->insertRow(2);

    ui->tableWidget->insertColumn(0);
    ui->tableWidget->insertColumn(1);
    ui->tableWidget->insertColumn(2);
    ui->tableWidget->insertColumn(3);

    //ui->listView->setItemDelegate(new BookViewDelegate(100, this));

    model->update();
}



MainWindow::~MainWindow()
{
    delete ui;
}
