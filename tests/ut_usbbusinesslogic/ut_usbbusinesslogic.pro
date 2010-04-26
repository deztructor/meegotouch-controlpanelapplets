SRC_PREFIX = ../../src/usbapplet
STUB_PREFIX = ../stubs
INCLUDEPATH += $$SRC_PREFIX \
               $$STUB_PREFIX

QT += \
    testlib \
    dbus 

PKGCONFIG += \
    usb_moded \
    dbus-1

TEMPLATE = app
DEFINES += UNIT_TEST
TARGET = ut_usbbusinesslogic
target.path = /usr/lib/systemui-applets-tests

CONFIG += \
	plugin \
	gui \
	meegotouch \
	duicontrolpanel \
	debug \
	link_pkgconfig \
	silent 

HEADERS += \
    ../../src/debug.h \
    ut_usbbusinesslogic.h \
    $$SRC_PREFIX/usbbusinesslogic.h

SOURCES += \
    ../../src/debug.cpp \
    ut_usbbusinesslogic.cpp \
    $$SRC_PREFIX/usbbusinesslogic.cpp

INSTALLS += target

