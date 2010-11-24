TEMPLATE = app
TARGET = soundsettings
DEPENDPATH += .
INCLUDEPATH += .

CONFIG +=           \
    meegotouch      \
    duicontrolpanel \
    meegocontrol    \
    qt              \
    silent

QT += gui

HEADERS += soundsettings.h
SOURCES += soundsettings.cpp

