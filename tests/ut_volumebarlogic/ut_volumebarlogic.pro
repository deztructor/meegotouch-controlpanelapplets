include(../coverage.pri)
include(../check.pri)

SRC_PREFIX = ../../src/volumebarplugin
INCLUDEPATH += $$SRC_PREFIX

QT += testlib

TEMPLATE = app

DEFINES += UNIT_TEST

TARGET = ut_volumebarlogic
target.path = /usr/lib/systemui-applets-tests

CONFIG += \
    link_pkgconfig \
	silent 

PKGCONFIG += dbus-1 dbus-glib-1

INCLUDEPATH += \
    /usr/include/dbus-1.0

HEADERS += \
    ../../src/debug.h \
    ut_volumebarlogic.h \
    $$SRC_PREFIX/volumebarlogic.h

SOURCES += \
    ../../src/debug.cpp \
    ut_volumebarlogic.cpp \
    $$SRC_PREFIX/volumebarlogic.cpp

INSTALLS += target
