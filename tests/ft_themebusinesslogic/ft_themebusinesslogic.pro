include(../coverage.pri)
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
TARGET = ft_themebusinesslogic
target.path = /usr/lib/systemui-applets-tests

HEADERS += \
    ../../src/debug.h \
    ft_themebusinesslogic.h \
    $$SRC_PREFIX/themebusinesslogic.h \
    $$SRC_PREFIX/themedescriptor.h

SOURCES += \
    ../../src/debug.cpp \
    ft_themebusinesslogic.cpp \
    $$SRC_PREFIX/themebusinesslogic.cpp \
    $$SRC_PREFIX/themedescriptor.cpp

INSTALLS += target
