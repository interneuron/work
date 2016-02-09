#-------------------------------------------------
#
# Project created by QtCreator 2015-01-14T16:07:17
#
#-------------------------------------------------

QT      += core gui
QT      += network widgets

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x000000
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = first
TEMPLATE = app
RC_FILE = myapp.rc
RC_FILE = icorc.rc


CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
        broadcast.cpp \
        changepass.cpp \
        passbox.cpp


HEADERS  += mainwindow.h \
    broadcast.h \
    changepass.h \
    passbox.h
    ui_mainwindow.h


FORMS    += mainwindow.ui \
         changepass.ui \
    passbox.ui

RESOURCES += \
    userdata.qrc

DISTFILES += \
    mind.txt \
    myapp.rc \
    good.txt \
    different.txt \
    icorc.rc


