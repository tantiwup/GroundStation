#-------------------------------------------------
#
# Project created by QtCreator 2015-10-30T16:20:15
#
#-------------------------------------------------

QT       += core gui
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SimulDraft2UAV
TEMPLATE = app


SOURCES += main.cpp \
    uav.cpp \
    link.cpp


HEADERS  += \
    uav.h \
    link.h

FORMS    += dialog.ui

DISTFILES +=
CONFIG += c++11

INCLUDEPATH += common/common/
