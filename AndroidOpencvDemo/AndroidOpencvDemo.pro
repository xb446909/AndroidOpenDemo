#-------------------------------------------------
#
# Project created by QtCreator 2015-12-26T12:55:14
#
#-------------------------------------------------

QT       += core gui androidextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AndroidOpencvDemo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    androidimagepicker.cpp

HEADERS  += mainwindow.h \
    androidimagepicker.h

FORMS    += mainwindow.ui

CONFIG += mobility
MOBILITY = 

