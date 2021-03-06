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
    subwindows/universalrelationtable.cpp \
    core/myscene.cpp \
    core/sceneElem/arrow.cpp \
    core/sceneElem/mytable.cpp \
    subwindows/dbnamedialog.cpp \
    subwindows/newdialog.cpp

HEADERS  += mainwindow.h \
    core/storage.h \
    core/algo/Normalization.h \
    subwindows/attribute.h \
    subwindows/connection.h \
    misc/Matrix.h \
    subwindows/universalrelationtable.h \
    core/myscene.h \
    core/sceneElem/arrow.h \
    core/sceneElem/mytable.h \
    subwindows/dbnamedialog.h \
    subwindows/newdialog.h


FORMS    += mainwindow.ui \
    subwindows/attribute.ui \
    subwindows/connection.ui \
    subwindows/universalrelationtable.ui \
    subwindows/dbnamedialog.ui \
    subwindows/newdialog.ui



CONFIG += c++11

OTHER_FILES += \
    pictures/key.bmp \
    pictures/key.png

RESOURCES += \
    pictures/res.qrc
