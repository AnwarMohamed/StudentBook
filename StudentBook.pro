#-------------------------------------------------
#
# Project created by QtCreator 2014-05-12T03:54:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = StudentBook
TEMPLATE = app


SOURCES += main.cpp \
    mainwindow.cpp \
    aboutdialog.cpp \
    studenttree.cpp \
    studentmodel.cpp \
    linkedlist.cpp

HEADERS  += \
    mainwindow.h \
    aboutdialog.h \
    studenttree.h \
    studentmodel.h \
    linkedlist.h

FORMS    += \
    mainwindow.ui \
    aboutdialog.ui

RESOURCES += \
    StudentBook.qrc
