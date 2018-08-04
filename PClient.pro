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
    mainwindow.cpp \
    clientthread.cpp \
    controller/fileutil.cpp \
    form/queryhistorydialog.cpp

HEADERS  += login.h \
    qss/qss.h \
    controller/database.h \
    mainwindow.h \
    clientthread.h \
    controller/fileutil.h \
    form/queryhistorydialog.h

FORMS    += login.ui \
    mainwindow.ui \
    form/queryhistorydialog.ui

RESOURCES += \
    app.qrc \
    qss/qss.qrc
