#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "filter.h"
#include "query.h"
#include "graphqlUrlGenerator.h"
#include <curl/curl.h>
#include <json.hpp>
using json = nlohmann::json;

#include "query.h"
#include "bookTable.h"
#include "listModel.h"
#include "graphQLLoader.h"
#include "listViewManager.h"

#include <QAbstractListModel>
#include <QStandardItemModel>
#include <QStringList>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QObject>
#include <fstream>
#include <QDebug>

using Data::Books::Book;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

struct PersonTemp {
    std::string name;
};

struct PersonTempSub : public PersonTemp {};

void to_json(nlohmann::json &j, const std::shared_ptr<PersonTemp> &p)
{
    j["name"] = p->name;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    std::string _url3 = "https://mylibrary.io/graphql-public?query=%7B%0A%20%20allBooks(PAGE%3A1%2C%20PAGE_SIZE%3A%2050%2C%20userId%3A%20%22HelloWorld%22)%7B%0A%20%20%20%20Books%7B%0A%20%20%20%20%20%20title%0A%20%20%20%20%20%20authors%0A%20%20%20%20%20%20_id%0A%20%20%20%20%20%20ean%0A%20%20%20%20%20%20smallImage%0A%20%20%20%20%20%20mediumImage%0A%20%20%20%20%20%20userId%0A%20%20%20%20%7D%0A%20%20%7D%0A%7D";
    std::string url3 = "https://mylibrary.io/graphql-public?query=%7B%0A%20%20allBooks(PAGE%3A1%2C%20PAGE_SIZE%3A%2050%2C%20userId%3A%20%22573d1b97120426ef0078aa92%22%2C%20SORT%3A%7Btitle%3A%201%7D)%7B%0A%20%20%20%20Books%7B%0A%20%20%20%20%20%20title%0A%20%20%20%20%20%20authors%0A%20%20%20%20%20%20_id%0A%20%20%20%20%20%20ean%0A%20%20%20%20%20%20smallImage%0A%20%20%20%20%20%20mediumImage%0A%20%20%20%20%20%20userId%0A%20%20%20%20%7D%0A%20%20%7D%0A%7D";

    std::string url4_base1 = "https://mylibrary.io/graphql-public?query=query%20allBooks(%24page%3A%20Int)%20%7B%20%0A%20%20allBooks(PAGE%3A%24page%2C%20PAGE_SIZE%3A%2050%2C%20userId%3A%20%22573d1b97120426ef0078aa92%22%2C%20SORT%3A%7Btitle%3A%201%7D)%7B%0A%20%20%20%20Books%7B%0A%20%20%20%20%20%20title%0A%20%20%20%20%20%20authors%0A%20%20%20%20%20%20_id%0A%20%20%20%20%20%20ean%0A%20%20%20%20%20%20smallImage%0A%20%20%20%20%20%20mediumImage%0A%20%20%20%20%20%20userId%0A%20%20%20%20%7D%0A%20%20%7D%0A%7D&operationName=allBooks&variables=";
    std::string url4_good1 = "https://mylibrary.io/graphql-public?query=query%20allBooks(%24page%3A%20Int)%20%7B%20%0A%20%20allBooks(PAGE%3A%24page%2C%20PAGE_SIZE%3A%2050%2C%20userId%3A%20%22573d1b97120426ef0078aa92%22%2C%20SORT%3A%7Btitle%3A%201%7D)%7B%0A%20%20%20%20Books%7B%0A%20%20%20%20%20%20title%0A%20%20%20%20%20%20authors%0A%20%20%20%20%20%20_id%0A%20%20%20%20%20%20ean%0A%20%20%20%20%20%20smallImage%0A%20%20%20%20%20%20mediumImage%0A%20%20%20%20%20%20userId%0A%20%20%20%20%7D%0A%20%20%7D%0A%7D&operationName=allBooks&variables=%7B%0A%20%20%22page%22%3A%202%0A%7D";
    std::string url4_test1 = "https://mylibrary.io/graphql-public?query=query%20allBooks(%24page%3A%20Int)%20%7B%20%0A%20%20allBooks(PAGE%3A%24page%2C%20PAGE_SIZE%3A%2050%2C%20userId%3A%20%22573d1b97120426ef0078aa92%22%2C%20SORT%3A%7Btitle%3A%201%7D)%7B%0A%20%20%20%20Books%7B%0A%20%20%20%20%20%20title%0A%20%20%20%20%20%20authors%0A%20%20%20%20%20%20_id%0A%20%20%20%20%20%20ean%0A%20%20%20%20%20%20smallImage%0A%20%20%20%20%20%20mediumImage%0A%20%20%20%20%20%20userId%0A%20%20%20%20%7D%0A%20%20%7D%0A%7D&operationName=allBooks&variables=%7B%22page%22%3A2%7D";

    std::string masterBase = "https://mylibrary.io/graphql-public?query=";
    std::string url4_query = "query allBooks($page:Int,$title_contains:String){allBooks(PAGE:$page,PAGE_SIZE:50,userId:\"573d1b97120426ef0078aa92\",SORT:{title:1},title_contains: $title_contains){Books{title authors _id ean smallImage mediumImage userId}}}";

//https://mylibrary.io/graphql-public?query=query%20allBooks%28%24page%3AInt%29%7BallBooks%28PAGE%3A%24page%2CPAGE_SIZE%3A50%2CuserId%3A%22573d1b97120426ef0078aa92%22%2CSORT%3A%7Btitle%3A1%7D%29%7BBooks%7Btitle%20authors%20_id%20ean%20smallImage%20mediumImage%20userId%7D%7D%7D%26operationName%3DallBooks%26variables%3D%7B%22page%22%3A2%7D

    //qDebug() << url4.c_str();

    json searchObject;
    searchObject["page"] = 1;

    std::string variables = searchObject.dump();
    qDebug() << variables.c_str();

    std::string url4 = url4_base1 + encode(variables);

    auto urlGenerator = GraphqlUrlGenerator::MakeUrlGenerator(url4_query);

    std::string final = urlGenerator(variables);
    qDebug() << url4.c_str() << "final:\n\n" << final.c_str() << "\n\n";


    using namespace Data::Books;

    auto q0 = makeFilter<Book>();

    ActualFilter foo { title, { std::string{"A"}, std::string{"B"} }, "in" };
    qDebug() << "\n\nDeduction Guide 1\n\n" << foo.serialize().c_str() << "\n\n";

    ActualFilter foo2 { title, std::initializer_list<std::string>{ "C", "D" }, "in" };
    qDebug() << "\n\nDeduction Guide 2\n\n" << foo2.serialize().c_str() << "\n\n";

    ActualFilter foo3 { title, { "E", "F" }, "in" };
    qDebug() << "\n\nDeduction Guide 3\n\n" << foo3.serialize().c_str() << "\n\n";

    nlohmann::json jsonTest;
    jsonTest["OR"] = std::vector<std::shared_ptr<PersonTemp>>{ std::make_shared<PersonTempSub>(PersonTempSub{{"Adam"}}), std::make_shared<PersonTempSub>(PersonTempSub{"Bob"}), std::make_shared<PersonTempSub>(PersonTempSub{"Matt"}) };
    qDebug() << "\n\nTEST\n\n" << jsonTest.dump().c_str() << "\n\n";

    auto q = makeFilter(
//        (weight < 10 || pages < 500) && (pages < 100 || weight < 90),

        title == "Hello World",
        authors.matches({"a", "b"}),
        authors.matches({"c", "d"}),
        title.in({ "t1", "t2" }),
        weight.in({ 2.2, 3.3, 4.4 }),
        title == "Title 1" || title == "Title 2",


        //Data::Subjects::name == "junk",


        //weight < 50 || weight < 100 || weight < 100,


        //weight < 50 || weight < 100 || weight < 110 || weight < 120,
        //weight < 10 || (weight < 50 || weight < 100),
        //(weight < 10 || weight < 50) || weight < 100,
        //(weight < 10 || weight < 50) || (weight < 100 || weight < 110),

        //weight < 50 && weight.as("weight2") < 100,
        //weight < 10 && (weight < 50 && weight < 100),
        //(weight < 10 && weight < 50) && weight < 100,

        //weight < 50 || (weight < 100 && weight < 200),
        //weight < 50 || weight < 60 || (weight < 100 && weight < 200),
        //(weight < 50 || weight < 60) || (weight < 100 && weight < 200),

        (weight < 50 || weight.as("weight2") < 100) && weight < 200,


//        title == "Hello World",
//        "World Hello" == smallImage,


        title.as("title2").in({ "title1", "title2" }) || weight < 50

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

                /*
        weight < 10 || weight < 50,
        //pages == 20,
        pages < 20,
        weight < 10 || (weight < 50 || weight < 100),
        weight < 10 || weight < 50 || weight < 100
                */
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

    ListViewManager<Book, BookListWidgetItem> *listViewManager = new ListViewManager<Book, BookListWidgetItem>{ ui->listView, 50 };


    GraphQLLoader<Book> *loader = new GraphQLLoader<Book> { [listViewManager](std::vector<Book> books) { listViewManager->setData(books); } };

    qDebug() << "||" << final.c_str() << "||";
    loader->load(final);

    ui->tableWidget->insertRow(0);
    ui->tableWidget->insertRow(1);
    ui->tableWidget->insertRow(2);

    ui->tableWidget->insertColumn(0);
    ui->tableWidget->insertColumn(1);
    ui->tableWidget->insertColumn(2);
    ui->tableWidget->insertColumn(3);



    connect(ui->pushButton, &QPushButton::released, [this, loader, urlGenerator, final](){
        this->ui->titleSearchLineEdit->text();
        json j;
        std::string search = this->ui->titleSearchLineEdit->text().toStdString();

        j["title_contains"] = search;
        //j["page"] = 2;

        qDebug() << "OLD" << "\n\n" << final.c_str() << "\n\n";
        qDebug() << "NEW" << "\n\n" << urlGenerator(j.dump()).c_str() << "\n\nJSON:\n\n" << j.dump().c_str() << "\n\n";
        loader->load(urlGenerator(j.dump()));

        //loader->load(final);
    });

    //ui->listView->setItemDelegate(new BookViewDelegate(100, this));

    //model->update();
}



MainWindow::~MainWindow()
{
    delete ui;
}
