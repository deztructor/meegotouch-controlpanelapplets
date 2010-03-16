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
	plugin \
	gui \
	dui \
	duicontrolpanel \
	debug \
	silent 

HEADERS += \
    ../../src/debug.h \
    ut_leddbusinterface.h \
    $$SRC_PREFIX/ledapplet.h \
    $$SRC_PREFIX/ledbrief.h \
    $$SRC_PREFIX/leddbusinterface.h \
    $$SRC_PREFIX/ledwidget.h

SOURCES += \
    ../../src/debug.cpp \
    ut_leddbusinterface.cpp \
    $$SRC_PREFIX/ledapplet.cpp \
    $$SRC_PREFIX/ledbrief.cpp \
    $$SRC_PREFIX/leddbusinterface.cpp \
    $$SRC_PREFIX/ledwidget.cpp

INSTALLS += target
