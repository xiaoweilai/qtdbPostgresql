#-------------------------------------------------
#
# Project created by QtCreator 2015-04-28T22:22:10
#
#-------------------------------------------------

QT       += core gui

TARGET = AutoCcodeHQ
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

#安装目录不能有空格，如果有空格，驱动会不完成。
#版本9.3
INCLUDEPATH += "D:\PostgreSQL\include"
LIBS += "D:\PostgreSQL\lib\libpq.lib"


