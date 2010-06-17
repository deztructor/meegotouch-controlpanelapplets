include(../coverage.pri)
include(../check.pri)

MOC_DIR = .moc
SRC_PREFIX = ../../src/themeapplet
STUB_PREFIX = ../stubs
INCLUDEPATH += $$SRC_PREFIX \
               $$STUB_PREFIX

CONFIG += \
	gui \
	meegotouch \
	debug \
	silent 

QT += \
    testlib \
    gui \
    dbus 

TEMPLATE = app
DEFINES += UNIT_TEST
TARGET = ut_themebusinesslogic
target.path = /usr/lib/systemui-applets-tests

HEADERS += \
    ../../src/debug.h \
    ut_themebusinesslogic.h \
    $$STUB_PREFIX/mdesktopentrystub.h \
    $$STUB_PREFIX/qdirstub.h \
    $$SRC_PREFIX/themebusinesslogic.h \
    $$SRC_PREFIX/themedescriptor.h

SOURCES += \
    ../../src/debug.cpp \
    ut_themebusinesslogic.cpp \
    $$STUB_PREFIX/mdesktopentrystub.cpp \
    $$SRC_PREFIX/themebusinesslogic.cpp \
    $$SRC_PREFIX/themedescriptor.cpp

INSTALLS += target
