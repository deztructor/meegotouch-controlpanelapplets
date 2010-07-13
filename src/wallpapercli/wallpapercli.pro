include(../coverage.pri)

TEMPLATE = app
TARGET = ./wallpapercli

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
    /usr/include/QtTracker \
    /usr/include/thumbnailer 

HEADERS = \
    ../debug.h \
    main.h

SOURCES = \
    ../debug.cpp \
    main.cpp


