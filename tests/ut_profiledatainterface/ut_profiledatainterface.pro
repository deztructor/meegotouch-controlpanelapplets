include(../common_top.pri)

SRC_PREFIX = ../../src/profileapplet
STUB_PREFIX = ../stubs

INCLUDEPATH += \
    $$SRC_PREFIX \
    $$STUB_PREFIX

QT += \
    testlib \
    dbus 

TEMPLATE = app
DEFINES += UNIT_TEST
TARGET = ut_profiledatainterface
target.path = /usr/lib/$$TEST_PKG_NAME

CONFIG += \
    gui \
    meegotouch \
    silent 

HEADERS += \
    $$STUB_PREFIX/profilestub.h \
    ../../src/debug.h \
    ut_profiledatainterface.h \
    $$SRC_PREFIX/profiledatainterface.h

SOURCES += \
    $$STUB_PREFIX/profilestub.cpp \
    ../../src/debug.cpp \
    ut_profiledatainterface.cpp \
    $$SRC_PREFIX/profiledatainterface.cpp

INSTALLS += target
