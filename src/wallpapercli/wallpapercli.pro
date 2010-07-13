include(../coverage.pri)

TEMPLATE = app
TARGET = ./wallpapercli
target.path = /usr/bin

SRC_PREFIX = ../wallpaperapplet

MOC_DIR = .moc
OBJECTS_DIR = .objects

CONFIG += \
    gui \
    meegotouch \
    duicontrolpanel \
    silent

LIBS += \
    -lqttracker \
    -lthumbnailer

INCLUDEPATH += \
    $$SRC_PREFIX \
    /usr/include/QtTracker \
    /usr/include/thumbnailer 

HEADERS = \
    ../debug.h \
    main.h \
    $$SRC_PREFIX/wallpapergconf.h

SOURCES = \
    ../debug.cpp \
    main.cpp

INSTALLS += target
