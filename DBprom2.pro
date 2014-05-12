#-------------------------------------------------
#
# Project created by QtCreator 2014-04-18T03:36:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DBprom2
TEMPLATE = app


SOURCES += main.cpp \
    mainwindow.cpp \
    core/storage.cpp \
    core/algo/algorithm.cpp \
    core/algo/Normalization.cpp \
    subwindows/attribute.cpp \
    subwindows/connection.cpp \
    misc/Matrix.cpp \
    misc/node.cpp \
    subwindows/universalrelationtable.cpp \
    core/myscene.cpp \
    core/sceneElem/arrow.cpp \
    core/sceneElem/mytable.cpp

HEADERS  += mainwindow.h \
    core/storage.h \
    core/algo/Normalization.h \
    subwindows/attribute.h \
    subwindows/connection.h \
    misc/Matrix.h \
    misc/node.h \
    subwindows/universalrelationtable.h \
    core/myscene.h \
    core/sceneElem/arrow.h \
    core/sceneElem/mytable.h


FORMS    += mainwindow.ui \
    subwindows/attribute.ui \
    subwindows/connection.ui \
    subwindows/universalrelationtable.ui



CONFIG += c++11

OTHER_FILES += \
    pictures/key.bmp \
    pictures/key.png

RESOURCES += \
    pictures/res.qrc
