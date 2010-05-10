include(../coverage.pri)
SRC_PREFIX = ../../src/batteryapplet
STUB_PREFIX = ../stubs
INCLUDEPATH += $$SRC_PREFIX \
               $$STUB_PREFIX
QT += \
    testlib \
    dbus 

TEMPLATE = app
DEFINES += UNIT_TEST
TARGET = ut_batterydbusinterface
target.path = /usr/lib/systemui-applets-tests

CONFIG += \
	gui \
	meegotouch \
	silent 

HEADERS += \
    ../../src/debug.h \
    ut_batterydbusinterface.h \
    $$SRC_PREFIX/batterydbusinterface.h

SOURCES += \
    ../../src/debug.cpp \
    ut_batterydbusinterface.cpp \
    $$SRC_PREFIX/batterydbusinterface.cpp

INSTALLS += target
