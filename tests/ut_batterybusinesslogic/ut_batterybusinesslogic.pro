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

TARGET = ut_batterybusinesslogic
target.path = /usr/lib/systemui-applets-tests

CONFIG += \
    qmsystem \
    meegotouch \
	silent 

HEADERS += \
    ut_batterybusinesslogic.h \
    $$SRC_PREFIX/batterybusinesslogic.h \
    $$SRC_PREFIX/../debug.h \
    $$STUB_PREFIX/stubbase.h \
    $$STUB_PREFIX/qmbattery_stub.h \
    $$STUB_PREFIX/qmdevicemode_stub.h

SOURCES += \
    ut_batterybusinesslogic.cpp \
    $$SRC_PREFIX/batterybusinesslogic.cpp \
    $$SRC_PREFIX/../debug.cpp \
    $$STUB_PREFIX/stubbase.cpp

INSTALLS += target
