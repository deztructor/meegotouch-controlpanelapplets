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
	plugin \
	gui \
	m \
	mcontrolpanel \
	debug \
	silent 

HEADERS += \
    ../../src/debug.h \
    ut_displaybusinesslogic.h \
    $$SRC_PREFIX/dcpdisplay.h \
    $$SRC_PREFIX/displayapplet.h \
    $$SRC_PREFIX/displaybrief.h \
    $$SRC_PREFIX/displaybusinesslogic.h \
    $$SRC_PREFIX/displaywidget.h

SOURCES += \
    ../../src/debug.cpp \
    ut_displaybusinesslogic.cpp \
    $$SRC_PREFIX/displayapplet.cpp \
    $$SRC_PREFIX/displaybrief.cpp \
    $$SRC_PREFIX/displaybusinesslogic.cpp \
    $$SRC_PREFIX/displaywidget.cpp

INSTALLS += target
