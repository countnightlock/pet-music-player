#-------------------------------------------------
#
# Project created by QtCreator 2016-08-02T14:17:44
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = musicplayer
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    lyrics.cpp \
    visualiser.cpp \
    playlistmodel.cpp \
    database.cpp

HEADERS  += mainwindow.h \
    httpdownloader.h \
    visualiser.h \
    universal_functions.h \
    playlistmodel.h \
    database.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += libcurl
