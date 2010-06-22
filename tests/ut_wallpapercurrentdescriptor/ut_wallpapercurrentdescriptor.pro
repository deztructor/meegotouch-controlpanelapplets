include(../coverage.pri)
include(../check.pri)

MOC_DIR = .moc
SRC_PREFIX = ../../src/wallpaperapplet
STUB_PREFIX = ../stubs
INCLUDEPATH += \
	$$SRC_PREFIX \
        $$STUB_PREFIX

QT += \
    testlib \
    dbus 

TEMPLATE = app
DEFINES += UNIT_TEST
TARGET = ut_wallpapercurrentdescriptor
target.path = /usr/lib/systemui-applets-tests

CONFIG += \
	plugin \
	gui \
	meegotouch \
	duicontrolpanel \
	silent 

HEADERS += \
    $$STUB_PREFIX/thumbnailerstub.h \
    $$STUB_PREFIX/trackerstub.h \
    $$STUB_PREFIX/mdesktopentrystub.h \
    ../../src/debug.h \
    ut_wallpapercurrentdescriptor.h \
    $$SRC_PREFIX/wallpaperitrans.h \
    $$SRC_PREFIX/wallpaperdescriptor.h \
    $$SRC_PREFIX/wallpapercurrentdescriptor.h 

SOURCES += \
    $$STUB_PREFIX/thumbnailerstub.cpp \
    $$STUB_PREFIX/trackerstub.cpp \
    $$STUB_PREFIX/mdesktopentrystub.cpp \
    ../../src/debug.cpp \
    ut_wallpapercurrentdescriptor.cpp \
    $$SRC_PREFIX/wallpaperitrans.cpp \
    $$SRC_PREFIX/wallpaperdescriptor.cpp \
    $$SRC_PREFIX/wallpapercurrentdescriptor.cpp 

INSTALLS += target

MOC_DIR=./.moc
