#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "filter.h"
#include <curl/curl.h>
#include <json.hpp>
using json = nlohmann::json;

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

    std::string url = "http://mylibrary.io/graphql-public?query=%7B%0A%20%20allBooks%7B%0A%20%20%20%20Books%7B%0A%20%20%20%20%20%20title%0A%20%20%20%20%20%20authors%0A%20%20%20%20%7D%0A%20%20%7D%0A%7D";
    std::string urlY = "http://mylibrary.io/graphql-public";
    std::string urlZ = "http://mylibrary.io/graphql-public?query=%7B%0A%20%20allLabelColors%7B%0A%20%20%20%20LabelColors%7B%0A%20%20%20%20%20%20backgroundColor%0A%20%20%20%20%7D%0A%20%20%7D%20%0A%7D";
    //auto res = system(command.c_str());

    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
      curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
      res = curl_easy_perform(curl);
      curl_easy_cleanup(curl);

      //std::string junk = readBuffer.c_str();


      ui->textEdit->setPlainText(QString { readBuffer.c_str() });


      auto j2 = json::parse(readBuffer);
      auto x = j2.begin();

      auto y = *x;

      auto z = y.begin();
      auto zz = *z;

      int i = 12;
    }


    //ui->textEdit->setText(QString { "Hello" });
    //ui->textEdit->setText(QString { "Yooooooo" });


    Field f1 { "field1" };
    std::string s = (f1 == "Hello World").val;
    QString qs { s.c_str() };
}

MainWindow::~MainWindow()
{
    delete ui;
}
