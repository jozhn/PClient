#-------------------------------------------------
#
# Project created by QtCreator 2018-07-25T13:57:56
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PClient
TEMPLATE = app


SOURCES += main.cpp\
        login.cpp \
    qss/qss.cpp \
    controller/database.cpp \
    mainwindow.cpp

HEADERS  += login.h \
    qss/qss.h \
    controller/database.h \
    mainwindow.h

FORMS    += login.ui \
    mainwindow.ui

RESOURCES += \
    app.qrc \
    qss/qss.qrc
