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

#��װĿ¼�����пո�����пո������᲻��ɡ�
#�汾9.3
INCLUDEPATH += "D:\PostgreSQL\include"
LIBS += "D:\PostgreSQL\lib\libpq.lib"


