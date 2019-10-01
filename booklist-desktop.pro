#-------------------------------------------------
#
# Project created by QtCreator 2019-07-12T18:10:23
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = booklist-desktop
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++17

LIBS += /usr/local/curl/lib/libcurl.dylib -lcurl


SOURCES += \
        DataDisplay/bookListWidgetItem.cpp \
        Filter/filterUtils.cpp \
        Query/graphqlUrlGenerator.cpp \
        Query/query.cpp \
        Util/fileLoader.cpp \
        main.cpp \
        mainwindow.cpp \

HEADERS += \
        DataDisplay/bookListWidgetItem.h \
        DataDisplay/listModel.h \
        DataDisplay/listViewItemDelegate.h \
        DataDisplay/listViewManager.h \
        DataDisplay/listWidgetItem.h \
        DataLoading/graphQLLoader.h \
        Field/field.h \
        Filter/actualFilter.h \
        Filter/filter.h \
        Filter/filterUtils.h \
        Filter/operatorLess.h \
        Filter/operatorOr.h \
        Filters/filter.h \
        MongoTypes/bookTable.h \
        Query/graphqlUrlGenerator.h \
        Query/query.h \
        Util/fileLoader.h \
        booktable.h \
        field.h \
        filterutils.h \
        listmodel.h \
        mainwindow.h \
        mongoquerybase.h \
        query.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


INCLUDEPATH += DataLoading
INCLUDEPATH += DataDisplay
INCLUDEPATH += Field
INCLUDEPATH += Filter
INCLUDEPATH += MongoTypes
INCLUDEPATH += Query
INCLUDEPATH += Util
INCLUDEPATH += /usr/local/include
INCLUDEPATH += /usr/local/Cellar/nlohmann_json/3.6.1/include/nlohmann

