#-------------------------------------------------
#
# Project created by QtCreator 2017-08-29T19:43:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sudoku
TEMPLATE = app


SOURCES += main.cpp\
        sudo.cpp \
    doku.cpp \
    action.cpp \
    producer.cpp \
    rd.cpp

HEADERS  += sudo.h \
    doku.h \
    action.h \
    producer.h \
    rd.h

FORMS    += sudo.ui

RESOURCES += \
    resource.qrc
