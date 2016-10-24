#-------------------------------------------------
#
# Project created by QtCreator 2016-10-24T11:47:12
#
#-------------------------------------------------

QT       += core gui
QT       += serialport



greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ReflowOvenControl
TEMPLATE = app


SOURCES += main.cpp\
        reflowcontroller.cpp \
    analoggauge.cpp \
    serialportreader.cpp

HEADERS  += reflowcontroller.h \
    analoggauge.h \
    serialportreader.h

FORMS    += reflowcontroller.ui

RESOURCES += \
    resources.qrc
