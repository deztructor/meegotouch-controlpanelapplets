include(../coverage.pri)
SRC_PREFIX = ../../src/displayapplet
STUB_PREFIX = ../stubs
INCLUDEPATH += $$SRC_PREFIX \
               $$STUB_PREFIX

QT += \
    testlib \
    dbus 

TEMPLATE = app
DEFINES += UNIT_TEST
TARGET = ut_displaybusinesslogic
target.path = /usr/lib/systemui-applets-tests

CONFIG += \
	qmsystem \
	gui \
	meegotouch \
	debug \
	silent 

HEADERS += \
    ../../src/debug.h \
    ut_displaybusinesslogic.h \
    $$SRC_PREFIX/displaybusinesslogic.h

SOURCES += \
    ../../src/debug.cpp \
    ut_displaybusinesslogic.cpp \
    $$SRC_PREFIX/displaybusinesslogic.cpp

INSTALLS += target
