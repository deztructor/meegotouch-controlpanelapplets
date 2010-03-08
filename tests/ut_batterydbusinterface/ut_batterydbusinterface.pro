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
	plugin \
	gui \
	dui \
	duicontrolpanel \
	debug \
	silent 

HEADERS += \
    ../../src/debug.h \
    ut_batterydbusinterface.h \
    $$SRC_PREFIX/batteryapplet.h \
    $$SRC_PREFIX/batterybrief.h \
    $$SRC_PREFIX/batterydbusinterface.h \
    $$SRC_PREFIX/batteryimage.h \
    $$SRC_PREFIX/batterywidget.h \
    $$SRC_PREFIX/dcpbattery.h \
    $$SRC_PREFIX/slidercontainer.h \
    $$SRC_PREFIX/timecontainer.h

SOURCES += \
    ../../src/debug.cpp \
    ut_batterydbusinterface.cpp \
    $$SRC_PREFIX/batteryapplet.cpp \
    $$SRC_PREFIX/batterybrief.cpp \
    $$SRC_PREFIX/batterydbusinterface.cpp \
    $$SRC_PREFIX/batteryimage.cpp \
    $$SRC_PREFIX/batterywidget.cpp \
    $$SRC_PREFIX/slidercontainer.cpp \
    $$SRC_PREFIX/timecontainer.cpp

INSTALLS += target
