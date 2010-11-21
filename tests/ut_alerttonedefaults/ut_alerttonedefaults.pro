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
TARGET = ut_alerttonedefaults
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
    ut_alerttonedefaults.h \
    $$SRC_PREFIX/debug.h \
    $$SRC_PREFIX/trackerconnection.h \
    $$SRC_PREFIX/alerttone.h \
    $$SRC_PREFIX/alerttonepreview.h \
    $$SRC_PREFIX/qtrackedvariant.h \
    $$SRC_PREFIX/qprofilevalue.h \
    $$SRC_PREFIX/alerttonedefaults.h \
    $$SRC_PREFIX/alerttonedefaultsmodel.h

SOURCES += \
    ut_alerttonedefaults.cpp  \
    $$SRC_PREFIX/debug.cpp \
    $$SRC_PREFIX/trackerconnection.cpp \
    $$SRC_PREFIX/alerttone.cpp \
    $$SRC_PREFIX/alerttonepreview.cpp \
    $$SRC_PREFIX/qtrackedvariant.cpp \
    $$SRC_PREFIX/qprofilevalue.cpp \
    $$SRC_PREFIX/alerttonedefaults.cpp \
    $$SRC_PREFIX/alerttonedefaultsmodel.cpp



INSTALLS += target


