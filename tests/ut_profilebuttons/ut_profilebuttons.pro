include(../coverage.pri)
include(../check.pri)

MOC_DIR = .moc
SRC_PREFIX = ../../src/profileplugin
STUB_PREFIX = ../stubs
INCLUDEPATH += $$SRC_PREFIX \
               $$STUB_PREFIX
QT += \
    testlib \
    dbus 

TEMPLATE = app

DEFINES += \
	UNIT_TEST 

TARGET = ut_profilebuttons
target.path = /usr/lib/systemui-applets-tests

CONFIG += \
	gui \
	meegotouch \
	plugin \
	duicontrolpanel \
	silent 

HEADERS += \
    ../../src/debug.h \
    ut_profilebuttons.h \
    $$SRC_PREFIX/profilebuttons.h 

SOURCES += \
    ../../src/debug.cpp \
    ut_profilebuttons.cpp \
    $$SRC_PREFIX/profilebuttons.cpp 

INSTALLS += target
