include(../coverage.pri)
include(../check.pri)

SRC_PREFIX = ../../src/displayapplet
STUB_PREFIX = ../stubs
INCLUDEPATH += $$SRC_PREFIX \
               $$STUB_PREFIX

CONFIG += \
	qmsystem \
	gui \
	meegotouch \
	silent 

QT += \
    testlib \
    dbus \
    gui

TEMPLATE = app
DEFINES += UNIT_TEST
TARGET = ut_displaybusinesslogic
target.path = /usr/lib/systemui-applets-tests

HEADERS += \
    ../../src/debug.h \
    ut_displaybusinesslogic.h \
    $$SRC_PREFIX/displaybusinesslogic.h

SOURCES += \
    ../../src/debug.cpp \
    ut_displaybusinesslogic.cpp \
    $$SRC_PREFIX/displaybusinesslogic.cpp

INSTALLS += target
