include(../common_top.pri)

MOC_DIR = .moc
SRC_PREFIX = ../../src/aboutapplet
STUB_PREFIX = ../stubs

INCLUDEPATH = \
        $$STUB_PREFIX \
	$$SRC_PREFIX \
	$$INCLUDEPATH

CONFIG += \
	gui \
	meegotouch \
	debug \
        link_pkgconfig \
	silent

PKGCONFIG += sysinfo

QT += \
    testlib \
    gui \
    dbus 

TEMPLATE = app
DEFINES += UNIT_TEST
TARGET = ut_aboutbusinesslogic
target.path = /usr/lib/$$TEST_PKG_NAME

HEADERS += \
    $$STUB_PREFIX/PhoneInfo \
    $$STUB_PREFIX/phoneinfo.h \
    ../../src/debug.h \
    ut_aboutbusinesslogic.h \
    $$SRC_PREFIX/aboutbusinesslogic.h 

SOURCES += \
    $$STUB_PREFIX/phoneinfo.cpp \
    ../../src/debug.cpp \
    ut_aboutbusinesslogic.cpp \
    $$SRC_PREFIX/aboutbusinesslogic.cpp 

INSTALLS += target
