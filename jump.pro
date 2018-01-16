#-------------------------------------------------
#
# Project created by QtCreator 2018-01-15T14:32:40
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia
CONFIG   += c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = jump
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    jumpp.cpp \
    autojump.cpp

HEADERS += \
        mainwindow.h \
    jumpp.h \
    autojump.h

FORMS += \
        mainwindow.ui
INCLUDEPATH += D:\pro\opencv\build\include \
               D:\pro\opencv\build\include\opencv  \
               D:\pro\opencv\build\include\opencv2

LIBS += D:\pro\opencv\build\x64\vc14\lib\opencv_world320.lib  \
        D:\pro\opencv\build\x64\vc14\lib\opencv_world320d.lib
