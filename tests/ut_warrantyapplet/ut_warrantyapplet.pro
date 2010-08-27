include(../coverage.pri)
include(../check.pri)

MOC_DIR = .moc
SRC_PREFIX = ../../src/warrantyapplet
STUB_PREFIX = ../stubs

INCLUDEPATH = \
        $$STUB_PREFIX \
	$$SRC_PREFIX \
	$$INCLUDEPATH

QT += \
    testlib \
    dbus 

TEMPLATE = app

DEFINES += \
	UNIT_TEST 

TARGET = ut_warrantyapplet
target.path = /usr/lib/systemui-applets-tests

CONFIG += \
	gui \
	meegotouch \
	plugin \
	qmsystem \
	duicontrolpanel \
	silent 

HEADERS += \
    $$STUB_PREFIX/mdesktopentry.h \
    ../../src/debug.h \
    ut_warrantyapplet.h \
    $$SRC_PREFIX/warrantyapplet.h \
    $$SRC_PREFIX/warrantybrief.h \
    $$SRC_PREFIX/warrantybusinesslogic.h \
    $$SRC_PREFIX/warrantywidget.h

SOURCES += \
    $$STUB_PREFIX/mdesktopentry.cpp \
    ../../src/debug.cpp \
    ut_warrantyapplet.cpp \
    $$SRC_PREFIX/warrantyapplet.cpp \
    $$SRC_PREFIX/warrantybrief.cpp \
    $$SRC_PREFIX/warrantybusinesslogic.cpp \
    $$SRC_PREFIX/warrantywidget.cpp 

INSTALLS += target
