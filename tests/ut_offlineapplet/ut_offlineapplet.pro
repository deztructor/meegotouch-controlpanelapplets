include(../coverage.pri)
include(../check.pri)

SRC_PREFIX = ../../src/offlineapplet
STUB_PREFIX = ../stubs
TOOLS_PREFIX = ../tools

#
# Sorry I made a qmdevicemode.h in the stub directory that causes some problems.
# FIXME: I need a better solution for this...
#
INCLUDEPATH = \
	/usr/include/qmsystem/ \
	$$INCLUDEPATH \
        $$STUB_PREFIX \
	$$SRC_PREFIX \
        $$TOOLS_PREFIX

QT += \
    testlib

TEMPLATE = app

DEFINES += \
	UNIT_TEST 

TARGET = ut_offlineapplet
target.path = /usr/lib/systemui-applets-tests

CONFIG += \
	gui \
	meegotouch \
        qmsystem \
	plugin \
	duicontrolpanel \
	silent 

HEADERS += \
    ut_offlineapplet.h \
    $$SRC_PREFIX/offlineapplet.h \
    $$SRC_PREFIX/offlinebrief.h \
    ../../src/debug.h \
    $$STUB_PREFIX/stubbase.h \
    $$STUB_PREFIX/qmdevicemode_stub.h \
    $$TOOLS_PREFIX/signalchecker.h



SOURCES += \
    ut_offlineapplet.cpp \
    ../../src/debug.cpp \
    $$SRC_PREFIX/offlineapplet.cpp \
    $$SRC_PREFIX/offlinebrief.cpp \
    $$STUB_PREFIX/stubbase.cpp \
    $$TOOLS_PREFIX/signalchecker.cpp

INSTALLS += target
