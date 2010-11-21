include(../common_top.pri)
include(../coverage.pri)
include(../check.pri)

SRC_PREFIX = ../../src/soundsettingsapplet
STUB_PREFIX = ../stubs
INCLUDEPATH += $$SRC_PREFIX \
               $$STUB_PREFIX
QT += \
    testlib \
    dbus

LIBS += -lqttracker
INCLUDEPATH += /usr/include/qt4/dui \
    /usr/include/duicontrolpanel

TEMPLATE = app
DEFINES += UNIT_TEST
TARGET = ut_alerttonetoplevel
target.path = /usr/lib/duicontrolpanel-soundsettingsapplet-tests

CONFIG += \
    plugin          \
    gui             \
    meegotouch      \
    debug           \
    link_pkgconfig  \
    duicontrolpanel

PKGCONFIG += meegotouch dbus-1 profile gconf-2.0 ContentManager gstreamer-0.10

HEADERS += \
    ut_alerttonetoplevel.h \
    $$SRC_PREFIX/debug.h \
    $$SRC_PREFIX/alerttonetoplevel.h  \
    $$SRC_PREFIX/qtrackedvariant.h

SOURCES += \
    ut_alerttonetoplevel.cpp \
    $$SRC_PREFIX/debug.cpp \
    $$SRC_PREFIX/alerttonetoplevel.cpp  \
    $$SRC_PREFIX/qtrackedvariant.cpp


INSTALLS += target


