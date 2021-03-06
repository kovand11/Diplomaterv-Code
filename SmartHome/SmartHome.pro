#-------------------------------------------------
#
# Project created by QtCreator 2017-04-16T20:41:44
#
#-------------------------------------------------

QT       += core gui sql network

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SmartHome
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    debuglinewidget.cpp \
    environmentalsensorwidget.cpp \
    linewidget.cpp \
    opendetectorwidget.cpp \
    wifisocketwidget.cpp \
    automationrule.cpp \
    devicehandler.cpp

HEADERS  += mainwindow.h \
    debuglinewidget.h \
    environmentalsensorwidget.h \
    linewidget.h \
    opendetectorwidget.h \
    wifisocketwidget.h \
    automationrule.h \
    devicehandler.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
