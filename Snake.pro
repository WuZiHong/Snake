#-------------------------------------------------
#
# Project created by QtCreator 2015-12-29T16:07:26
#
#-------------------------------------------------

QT       += core gui
QT += multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Snake
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    snakewin.cpp \
    calthread.cpp

HEADERS  += mainwindow.h \
    snakewin.h \
    calthread.h

FORMS    += mainwindow.ui

RESOURCES += \
    snakefile.qrc
