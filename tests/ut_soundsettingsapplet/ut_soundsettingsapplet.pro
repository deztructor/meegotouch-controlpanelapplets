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
TARGET = ut_soundsettingsapplet
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
    ut_soundsettingsapplet.h \
    $$SRC_PREFIX/debug.h \
    $$SRC_PREFIX/drilldownitem.h \
    $$SRC_PREFIX/trackerconnection.h \
    $$SRC_PREFIX/alerttone.h \
    $$SRC_PREFIX/alerttonewidget.h \
    $$SRC_PREFIX/alerttonetoplevel.h \
    $$SRC_PREFIX/alerttonepreview.h \
    $$SRC_PREFIX/alerttonevolume.h \
    $$SRC_PREFIX/alerttoneappletmaps.h \
    $$SRC_PREFIX/alerttoneappletwidget.h \
    $$SRC_PREFIX/alerttonebrowser.h \
    $$SRC_PREFIX/alerttonedefaults.h \
    $$SRC_PREFIX/alerttonedefaultsmodel.h \
    $$SRC_PREFIX/qtrackedvariant.h \
    $$SRC_PREFIX/qprofilevalue.h \
    $$SRC_PREFIX/qgconfdirmanager.h \
    $$SRC_PREFIX/gconfstringcombo.h \
    $$SRC_PREFIX/profileintcombo.h \
    $$SRC_PREFIX/qgconfvalue.h \
    $$SRC_PREFIX/static.h \
    $$SRC_PREFIX/soundsettingsapplet.h



SOURCES += \
    ut_soundsettingsapplet.cpp  \
    $$SRC_PREFIX/debug.cpp \
    $$SRC_PREFIX/drilldownitem.cpp \
    $$SRC_PREFIX/trackerconnection.cpp \
    $$SRC_PREFIX/alerttone.cpp \
    $$SRC_PREFIX/alerttonewidget.cpp \
    $$SRC_PREFIX/alerttonetoplevel.cpp \
    $$SRC_PREFIX/alerttonepreview.cpp \
    $$SRC_PREFIX/alerttonevolume.cpp \
    $$SRC_PREFIX/alerttoneappletmaps.cpp \
    $$SRC_PREFIX/alerttoneappletwidget.cpp \
    $$SRC_PREFIX/alerttonebrowser.cpp \
    $$SRC_PREFIX/alerttonedefaults.cpp \
    $$SRC_PREFIX/alerttonedefaultsmodel.cpp \
    $$SRC_PREFIX/qtrackedvariant.cpp \
    $$SRC_PREFIX/qprofilevalue.cpp \
    $$SRC_PREFIX/qgconfdirmanager.cpp \
    $$SRC_PREFIX/gconfstringcombo.cpp \
    $$SRC_PREFIX/profileintcombo.cpp \
    $$SRC_PREFIX/qgconfvalue.cpp \
    $$SRC_PREFIX/soundsettingsapplet.cpp



INSTALLS += target



