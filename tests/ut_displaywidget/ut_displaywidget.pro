include(../coverage.pri)
include(../check.pri)

SRC_PREFIX = ../../src/displayapplet
STUB_PREFIX = ../stubs

INCLUDEPATH = \
        $$STUB_PREFIX \
	$$SRC_PREFIX \
	$$INCLUDEPATH

CONFIG += \
	gui \
	meegotouch \
	plugin \
	duicontrolpanel \
	silent 

QT += \
    testlib \
    dbus \
    gui

TEMPLATE = app
DEFINES += UNIT_TEST
TARGET = ut_displaywidget
target.path = /usr/lib/systemui-applets-tests

HEADERS += \
    $$STUB_PREFIX/qmdisplaystate.h \
    ../../src/debug.h \
    ut_displaywidget.h \
    $$SRC_PREFIX/displaybusinesslogic.h \
    $$SRC_PREFIX/displaywidget.h

SOURCES += \
    $$STUB_PREFIX/qmdisplaystate.cpp \
    ../../src/debug.cpp \
    ut_displaywidget.cpp \
    $$SRC_PREFIX/displaybusinesslogic.cpp \
    $$SRC_PREFIX/displaywidget.cpp

INSTALLS += target
