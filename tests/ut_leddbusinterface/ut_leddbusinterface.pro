include(../coverage.pri)
SRC_PREFIX = ../../src/ledapplet
STUB_PREFIX = ../stubs
INCLUDEPATH += $$SRC_PREFIX \
               $$STUB_PREFIX
QT += \
    testlib \
    dbus 

TEMPLATE = app
DEFINES += UNIT_TEST
TARGET = ut_leddbusinterface
target.path = /usr/lib/systemui-applets-tests

CONFIG += \
	gui \
	meegotouch \
	debug \
	silent 

HEADERS += \
    ../../src/debug.h \
    ut_leddbusinterface.h \
    $$SRC_PREFIX/leddbusinterface.h

SOURCES += \
    ../../src/debug.cpp \
    ut_leddbusinterface.cpp \
    $$SRC_PREFIX/leddbusinterface.cpp

INSTALLS += target
