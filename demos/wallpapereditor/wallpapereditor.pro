TEMPLATE = app
TARGET = test
DEPENDPATH += .
INCLUDEPATH += .

CONFIG +=           \
    meegotouch      \
    duicontrolpanel \
    meegocontrol    \
    qt              \
    silent

QT += gui

HEADERS += wallpapereditor.h
SOURCES += wallpapereditor.cpp

