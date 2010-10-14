include(../common_top.pri)

MOC_DIR = .moc
SRC_PREFIX = ../../src/displayapplet
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

TARGET = ut_displayapplet
target.path = /usr/lib/$$TEST_PKG_NAME

CONFIG += \
	gui \
	meegotouch \
	plugin \
	duicontrolpanel \
	silent 

HEADERS += \
    $$STUB_PREFIX/qmdisplaystate.h \
    ../../src/debug.h \
    ut_displayapplet.h \
    $$SRC_PREFIX/dcpdisplay.h \
    $$SRC_PREFIX/displayapplet.h \
    $$SRC_PREFIX/displaybrief.h \
    $$SRC_PREFIX/displaybusinesslogic.h \
    $$SRC_PREFIX/displaywidget.h


SOURCES += \
    $$STUB_PREFIX/qmdisplaystate.cpp \
    ../../src/debug.cpp \
    ut_displayapplet.cpp \
    $$SRC_PREFIX/displayapplet.cpp \
    $$SRC_PREFIX/displaybrief.cpp \
    $$SRC_PREFIX/displaybusinesslogic.cpp \
    $$SRC_PREFIX/displaywidget.cpp

INSTALLS += target
