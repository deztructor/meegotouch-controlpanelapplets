include(../common_top.pri)

SRC_PREFIX = ../../src/aboutapplet
STUB_PREFIX = ../stubs

INCLUDEPATH += \
    $$SRC_PREFIX 

CONFIG += \
    gui \
    mobility \
    meegotouch \
    link_pkgconfig \
    silent

QT += \
    testlib \
    dbus \
    gui

MOBILITY += \
    systeminfo

TEMPLATE = app

DEFINES += \
    UNIT_TEST \
    FUNCTIONAL_TEST	

TARGET = ft_aboutbusinesslogic
target.path = /usr/lib/$$TEST_PKG_NAME

HEADERS += \
    ../../src/debug.h \
    ft_aboutbusinesslogic.h \
    $$SRC_PREFIX/aboutbusinesslogic.h

SOURCES += \
    ../../src/debug.cpp \
    ft_aboutbusinesslogic.cpp \
    $$SRC_PREFIX/aboutbusinesslogic.cpp

INSTALLS += target
