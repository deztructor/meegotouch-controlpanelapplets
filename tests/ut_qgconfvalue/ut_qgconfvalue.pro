include(../common_top.pri)

SRC_PREFIX = ../../src/soundsettingsapplet
STUB_PREFIX = ../stubs
INCLUDEPATH += $$SRC_PREFIX \
               $$STUB_PREFIX
QT += \
    testlib \
    dbus

LIBS += -lqttracker

TEMPLATE = app
DEFINES += UNIT_TEST
TARGET = ut_qgconfvalue
target.path = /usr/lib/$$TEST_PKG_NAME

CONFIG += \
    plugin          \
    gui             \
    meegotouchcore  \
    debug           \
    silent          \
    link_pkgconfig  \
    duicontrolpanel

PKGCONFIG += dbus-1 profile gconf-2.0 ContentManager gstreamer-0.10

HEADERS += \
    ut_qgconfvalue.h \
    $$SRC_PREFIX/../debug.h \
    $$SRC_PREFIX/qgconfdirmanager.h \
    $$SRC_PREFIX/qtrackedvariant.h \
    $$SRC_PREFIX/qgconfvalue.h

SOURCES += \
    ut_qgconfvalue.cpp  \
    $$SRC_PREFIX/../debug.cpp \
    $$SRC_PREFIX/qgconfdirmanager.cpp \
    $$SRC_PREFIX/qtrackedvariant.cpp \
    $$SRC_PREFIX/qgconfvalue.cpp

INSTALLS += target

