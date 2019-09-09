#include "mainwindow.h"
#include <QApplication>

template <typename T>
void print(T val)
{
    std::cout<<val;
}


int main(int argc, char *argv[])
{
    std::string str { "Hello" };
    int i = 12;

    std::cout<<str;
    //std::cout<<std::ref(str);
    std::cout<<static_cast<std::string>(std::ref(str));

    std::cout<<i;
    std::cout<<std::ref(i);

    print(i);
    print(std::ref(i));

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
