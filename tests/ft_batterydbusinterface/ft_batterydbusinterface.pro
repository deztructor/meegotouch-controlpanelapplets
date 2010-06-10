include(../coverage.pri)
include(../check.pri)

SRC_PREFIX = ../../src/batteryapplet
STUB_PREFIX = ../stubs
INCLUDEPATH += $$SRC_PREFIX \
               $$STUB_PREFIX
QT += \
    testlib \
    dbus 

TEMPLATE = app

DEFINES += \
	UNIT_TEST \
	FUNCTIONAL_TEST

TARGET = ft_batterydbusinterface
target.path = /usr/lib/systemui-applets-tests

CONFIG += \
	gui \
	meegotouch \
	silent 

HEADERS += \
    ../../src/debug.h \
    ft_batterydbusinterface.h \
    $$SRC_PREFIX/batterydbusinterface.h

SOURCES += \
    ../../src/debug.cpp \
    ft_batterydbusinterface.cpp \
    $$SRC_PREFIX/batterydbusinterface.cpp

INSTALLS += target
