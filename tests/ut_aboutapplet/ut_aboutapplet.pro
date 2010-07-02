include(../coverage.pri)
include(../check.pri)

MOC_DIR = .moc
SRC_PREFIX = ../../src/aboutapplet
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

TARGET = ut_aboutapplet
target.path = /usr/lib/systemui-applets-tests

CONFIG += \
    gui \
    meegotouch \
    plugin \
    duicontrolpanel \
    link_pkgconfig \
    silent 

PKGCONFIG += sysinfo

HEADERS += \
    $$STUB_PREFIX/mdesktopentry.h \
    $$STUB_PREFIX/phoneinfo.h \
    $$STUB_PREFIX/PhoneInfo \
    ../../src/debug.h \
    ut_aboutapplet.h \
    $$SRC_PREFIX/aboutapplet.h \
    $$SRC_PREFIX/aboutbrief.h \
    $$SRC_PREFIX/aboutbusinesslogic.h \
    $$SRC_PREFIX/aboutwidget.h

SOURCES += \
    $$STUB_PREFIX/mdesktopentry.cpp \
    $$STUB_PREFIX/phoneinfo.cpp \
    ../../src/debug.cpp \
    ut_aboutapplet.cpp \
    $$SRC_PREFIX/aboutapplet.cpp \
    $$SRC_PREFIX/aboutbrief.cpp \
    $$SRC_PREFIX/aboutbusinesslogic.cpp \
    $$SRC_PREFIX/aboutwidget.cpp

INSTALLS += target
