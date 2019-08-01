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

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

template <typename T>
struct StripFromPointer{};

template <typename T>
struct StripFromPointer<std::shared_ptr<T>>
{
    using Type = T;
};

using namespace Data::Books;

void F1(std::shared_ptr<Filter<Books>> af){}

std::function<void(std::shared_ptr<ActualFilter<Books, int>>)> f;



std::string test(std::shared_ptr<Filter<Books>> af)
{
    using Type = typename StripFromPointer<std::shared_ptr<Filter<Data::Books::Books>>>::Type;

    return typeid (Type).name();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    f = F1;

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

    std::shared_ptr<Filter<Books>> af = pages < 100;
    std::string S = test(af);

    auto q = makeFilter(
        //title == "Hello World",
        (weight < 10) || (weight < 50),
        //(weight < 10 || pages < 500) && (pages < 100 || weight < 90),
        //weight < 10 || (weight < 50 || weight < 100),
        //title == "Hello World",
        //"World Hello" == smallImage,
        //title.in({ "title1", "title2" }),
//        pages == 20,
//        20 == pages,

        pages < 20,
//        20 < pages,
//        19 == edition,

        weight < 9.5
//        9.5 < weight,
//        weight < 9,   //coerce to double!
//        9 < weight,   //coerce to double!

//        authors.matches({ "author 1", "author 2" }),

//        title.as("titleAlias") == "Hello World",
//        "Hello World" == title.as("titleAlias2"),
//        pages == 20,
//        pages < 20
//        //weight < 10 || weight < 50 || weight < 100
    );

    //std::shared_ptr<Filter<Books>> x = weight < 10 || weight < 50;
    //std::shared_ptr<ActualFilter<Books, double>> x = weight < 10;
    //std::shared_ptr<ActualFilter<Books, double>> y = weight < 10;



    //Query<Books> qb{ pages == 20, FilterList<Books>{ weight < 10, weight < 50 } };
    Query<Books> qb{ pages == 20, std::make_shared<OrFilter<Books>>(OrFilter<Books>{ weight < 10, weight < 50 }) };

    std::string message = S + "\n\n";
    for (const auto &el : q.filters)
    {
        message += el->serialize() + "\n";
    }


    ui->textEdit->setPlainText(QString { (message + "\n\n" + q.serialize()).c_str() });
}

MainWindow::~MainWindow()
{
    delete ui;
}
