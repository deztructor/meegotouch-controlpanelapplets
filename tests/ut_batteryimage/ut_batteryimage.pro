include(../common_top.pri)

MOC_DIR = .moc
SRC_PREFIX = ../../src/batteryapplet
STUB_PREFIX = ../stubs

INCLUDEPATH = \
        $$STUB_PREFIX \
	$$SRC_PREFIX \
	$$INCLUDEPATH

QT += testlib

TEMPLATE = app

DEFINES += UNIT_TEST 

TARGET = ut_batteryimage
target.path = /usr/lib/systemui-applets-tests

CONFIG += \
    meegotouch \
    silent 

contains(DEFINES, HAVE_QMSYSTEM) {
    CONFIG += qmsystem
}

HEADERS += \
    ut_batteryimage.h \
    $$SRC_PREFIX/../debug.h \
    $$SRC_PREFIX/batteryimage.h 

SOURCES += \
    ut_batteryimage.cpp \
    $$SRC_PREFIX/../debug.cpp \
    $$SRC_PREFIX/batteryimage.cpp 

INSTALLS += target
