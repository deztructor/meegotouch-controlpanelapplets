include(../common_top.pri)

MOC_DIR = .moc
SRC_PREFIX = ../../src/resetapplet
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

TARGET = ut_resetapplet
target.path = /usr/lib/$$TEST_PKG_NAME

CONFIG += \
	gui \
	meegotouch \
	plugin \
	duicontrolpanel \
	silent 

HEADERS += \
    $$STUB_PREFIX/mdesktopentry.h \
    ../../src/debug.h \
    ut_resetapplet.h \
    $$SRC_PREFIX/resetapplet.h \
    $$SRC_PREFIX/resetbrief.h \
    $$SRC_PREFIX/resetbusinesslogic.h \
    $$SRC_PREFIX/resetwidget.h

SOURCES += \
    $$STUB_PREFIX/mdesktopentry.cpp \
    ../../src/debug.cpp \
    ut_resetapplet.cpp \
    $$SRC_PREFIX/resetapplet.cpp \
    $$SRC_PREFIX/resetbrief.cpp \
    $$SRC_PREFIX/resetbusinesslogic.cpp \
    $$SRC_PREFIX/resetwidget.cpp

INSTALLS += target
