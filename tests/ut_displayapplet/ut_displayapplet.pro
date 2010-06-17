include(../coverage.pri)
include(../check.pri)

MOC_DIR = .moc
SRC_PREFIX = ../../src/displayapplet
STUB_PREFIX = ../stubs
INCLUDEPATH += $$SRC_PREFIX \
               $$STUB_PREFIX
QT += \
    testlib \
    dbus 

TEMPLATE = app

DEFINES += \
	UNIT_TEST 

TARGET = ut_displayapplet
target.path = /usr/lib/systemui-applets-tests

CONFIG += \
	gui \
	meegotouch \
	plugin \
	duicontrolpanel \
	silent 

HEADERS += \
    $$STUB_PREFIX/qmdisplaystatestub.h \
    ../../src/debug.h \
    ut_displayapplet.h \
    $$SRC_PREFIX/dcpdisplay.h \
    $$SRC_PREFIX/displayapplet.h \
    $$SRC_PREFIX/displaybrief.h \
    $$SRC_PREFIX/displaybusinesslogic.h \
    $$SRC_PREFIX/displaywidget.h


SOURCES += \
    $$STUB_PREFIX/qmdisplaystatestub.cpp \
    ../../src/debug.cpp \
    ut_displayapplet.cpp \
    $$SRC_PREFIX/displayapplet.cpp \
    $$SRC_PREFIX/displaybrief.cpp \
    $$SRC_PREFIX/displaybusinesslogic.cpp \
    $$SRC_PREFIX/displaywidget.cpp

INSTALLS += target