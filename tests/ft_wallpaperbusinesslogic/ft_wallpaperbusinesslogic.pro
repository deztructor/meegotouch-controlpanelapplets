include(../coverage.pri)
SRC_PREFIX = ../../src/wallpaperapplet
STUB_PREFIX = ../stubs
INCLUDEPATH += $$SRC_PREFIX \
               $$STUB_PREFIX

QT += \
    testlib \
    dbus 

LIBS += \
    -lqttracker \
    -lduithumbnailer

INCLUDEPATH += \
    /usr/include/QtTracker \
    /usr/include/duithumbnailer 

TEMPLATE = app
DEFINES += UNIT_TEST
TARGET = ft_wallpaperbusinesslogic
target.path = /usr/lib/systemui-applets-tests

CONFIG += \
	plugin \
	gui \
	meegotouch \
	duicontrolpanel \
	silent 

HEADERS += \
    ../../src/debug.h \
    ft_wallpaperbusinesslogic.h \
    $$SRC_PREFIX/wallpaperapplet.h \
    $$SRC_PREFIX/wallpaperbrief.h \
    $$SRC_PREFIX/wallpaperbusinesslogic.h \
    $$SRC_PREFIX/wallpaperdescriptor.h \
    $$SRC_PREFIX/wallpapercurrentdescriptor.h \
    $$SRC_PREFIX/wallpaperinfoheader.h \
    $$SRC_PREFIX/wallpapereditorwidget.h \
    $$SRC_PREFIX/wallpaperlist.h \
    $$SRC_PREFIX/wallpapermodel.h \
    $$SRC_PREFIX/wallpaperitrans.h \
    $$SRC_PREFIX/wallpaperwidget.h

SOURCES += \
    ../../src/debug.cpp \
    ft_wallpaperbusinesslogic.cpp \
    $$SRC_PREFIX/wallpaperapplet.cpp \
    $$SRC_PREFIX/wallpaperbrief.cpp \
    $$SRC_PREFIX/wallpaperbusinesslogic.cpp \
    $$SRC_PREFIX/wallpaperdescriptor.cpp \
    $$SRC_PREFIX/wallpapercurrentdescriptor.cpp \
    $$SRC_PREFIX/wallpaperinfoheader.cpp \
    $$SRC_PREFIX/wallpapereditorwidget.cpp \
    $$SRC_PREFIX/wallpaperlist.cpp \
    $$SRC_PREFIX/wallpapermodel.cpp \
    $$SRC_PREFIX/wallpaperitrans.cpp \
    $$SRC_PREFIX/wallpaperwidget.cpp

INSTALLS += target
