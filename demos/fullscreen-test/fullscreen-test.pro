TEMPLATE = app
TARGET = test
DEPENDPATH += .
INCLUDEPATH += .

CONFIG +=           \
    meegotouch      \
    qt              \
    silent

QT += gui

HEADERS += fullscreen-test.h
SOURCES += fullscreen-test.cpp

