include(../coverage.pri)
include(../check.pri)

MOC_DIR = .moc
SRC_PREFIX = ../../src/batteryapplet
STUB_PREFIX = ../stubs
INCLUDEPATH += $$SRC_PREFIX \
               $$STUB_PREFIX
QT += testlib

TEMPLATE = app

DEFINES += UNIT_TEST 

TARGET = ut_batteryimage
target.path = /usr/lib/systemui-applets-tests

CONFIG += \
    qmsystem \
    meegotouch \
	silent 

HEADERS += \
    ut_batteryimage.h \
    $$SRC_PREFIX/../debug.h \
    $$SRC_PREFIX/batteryimage.h 

SOURCES += \
    ut_batteryimage.cpp \
    $$SRC_PREFIX/../debug.cpp \
    $$SRC_PREFIX/batteryimage.cpp 

INSTALLS += target
