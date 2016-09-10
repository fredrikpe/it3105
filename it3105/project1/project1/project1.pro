#-------------------------------------------------
#
# Project created by QtCreator 2016-09-09T11:42:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = project1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    board.cpp \
    backtracking.cpp

HEADERS  += mainwindow.h \
    board.h \
    backtracking.h

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS += -std=c++11
