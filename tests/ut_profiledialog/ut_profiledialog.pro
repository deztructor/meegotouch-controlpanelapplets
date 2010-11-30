include(../common_top.pri)

MOC_DIR = .moc
SRC_PREFIX = ../../src/profileplugin
APPLET_PREFIX = ../../src/profileapplet
STUB_PREFIX = ../stubs
INCLUDEPATH += $$SRC_PREFIX \
               $$STUB_PREFIX \
               $$APPLET_PREFIX

QT += \
    testlib \
    dbus

TEMPLATE = app

DEFINES += \
	UNIT_TEST

TARGET = ut_profiledialog
target.path = /usr/lib/$$TEST_PKG_NAME

CONFIG += \
	gui \
	meegotouch \
	plugin \
	duicontrolpanel \
    link_pkgconfig \
	silent

PKGCONFIG += \
    profile \
    dbus-1

HEADERS += \
    $$STUB_PREFIX/profilestub.h \
    ../../src/debug.h \
    ut_profiledialog.h \
    $$APPLET_PREFIX/profilebackend.h \
    $$APPLET_PREFIX/profiledatainterface.h \
    $$SRC_PREFIX/profiledialog.h

SOURCES += \
    $$STUB_PREFIX/profilestub.cpp \
    ../../src/debug.cpp \
    ut_profiledialog.cpp \
    $$APPLET_PREFIX/profilebackend.cpp \
    $$APPLET_PREFIX/profiledatainterface.cpp \
    $$SRC_PREFIX/profiledialog.cpp

INSTALLS += target
