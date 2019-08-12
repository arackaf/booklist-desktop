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
public:
    ListModel(QObject *parent) : QAbstractListModel(parent), books({ Book{}, Book{}, Book{} }) {}
    int rowCount(const QModelIndex &) const override;

    QVariant data(const QModelIndex &index, int = Qt::DisplayRole) const override;
};

int ListModel::rowCount(const QModelIndex &) const
{
    return 3;
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
     BookViewDelegate(int height, QObject* parent = 0) : QStyledItemDelegate(parent), height(height) {}

     int height;

     QSize sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const
     {
         return QSize(0, height); //enter your values here
     }

     inline void paintXXX(QPainter *painter, const QStyleOptionViewItem &option,
                       const QModelIndex &index ) const
     {
         // Set up a QStyleOptionProgressBar to precisely mimic the
         // environment of a progress bar.
         QStyleOptionButton boptions;
         boptions.text = "My Buttonnnnnnnnn";
         boptions.rect = option.rect;


         // Draw the progress bar onto the view.
         QApplication::style()->drawControl(QStyle::CE_PushButton, &boptions, painter);
     }
};

template <typename T>
struct MyString
{
    MyString(T) {}
    MyString() {}
    //MyString(T){}
};

template <typename T>
MyString<T> truncate(MyString<T> const &a, int len)
{
    return MyString<T>{};
}

int truncate(int a, int b)
{
    return a + b;
}

void test()
{
    MyString<int> str1;
    //str1 = truncate<char>("Hello", 5);
    truncate(1, 5);
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    //double resB = add(1, 2);


    ui->setupUi(this);

    std::string url = "https://mylibrary.io/graphql-public?query=%7B%0A%20%20allBooks%7B%0A%20%20%20%20Books%7B%0A%20%20%20%20%20%20title%0A%20%20%20%20%20%20authors%0A%20%20%20%20%7D%0A%20%20%7D%0A%7D";
    std::string url2 = "https://mylibrary.io/graphql-public?query=%7B%0A%20%20allBooks(PAGE%3A1%2C%20PAGE_SIZE%3A%2050)%7B%0A%20%20%20%20Books%7B%0A%20%20%20%20%20%20title%0A%20%20%20%20%20%20authors%0A%20%20%20%20%7D%0A%20%20%7D%0A%7D";
    std::string url3 = "https://mylibrary.io/graphql-public?query=%7B%0A%20%20allBooks(PAGE%3A1%2C%20PAGE_SIZE%3A%205)%7B%0A%20%20%20%20Books%7B%0A%20%20%20%20%20%20title%0A%20%20%20%20%20%20authors%0A%20%20%20%20%20%20_id%0A%20%20%20%20%20%20ean%0A%20%20%20%20%20%20smallImage%0A%20%20%20%20%20%20mediumImage%0A%20%20%20%20%20%20userId%0A%20%20%20%20%7D%0A%20%20%7D%0A%7D";
    std::string url4 = "https://mylibrary.io/graphql-public?query=%7B%0A%20%20allBooks%7B%0A%20%20%20%20Books%7B%0A%20%20%20%20%20%20title%0A%20%20%20%20%20%20authors%0A%20%20%20%20%20%20_id%0A%20%20%20%20%20%20ean%0A%20%20%20%20%20%20smallImage%0A%20%20%20%20%20%20mediumImage%0A%20%20%20%20%20%20userId%0A%20%20%20%20%7D%0A%20%20%7D%0A%7D";
    //auto res = system(command.c_str());

    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(false && curl) {
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
      auto x = j2.begin();

      auto y = *x;

      auto z = y.begin();
      auto zz = *z;

      int i = 12;
    }

    using namespace Data::Books;


    auto q = makeFilter(
//        (weight < 10 || pages < 500) && (pages < 100 || weight < 90),
        title == "Hello World",
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


    auto w = new QWidget();
    auto v = new QVBoxLayout{};
    v->addWidget(new QPushButton{"Heyooooo"});
    v->addWidget(new QLabel{"Hi there"});
    v->addWidget(new QPushButton{"Button 2"});

    w->setLayout(v);


    ui->listView->setIndexWidget(model->index(0), w);
    ui->listView->setItemDelegate(new BookViewDelegate(v->sizeHint().height(), this));
}




MainWindow::~MainWindow()
{
    delete ui;
}
