include(../coverage.pri)
include(../check.pri)

MOC_DIR = .moc
SRC_PREFIX = ../../src/themeapplet
STUB_PREFIX = ../stubs

INCLUDEPATH = \
        $$STUB_PREFIX \
	$$SRC_PREFIX \
	$$INCLUDEPATH

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
TARGET = ut_themecellcreator
target.path = /usr/lib/systemui-applets-tests

HEADERS += \
    $$STUB_PREFIX/mdesktopentry.h \
    ../../src/debug.h \
    ut_themecellcreator.h \
    $$STUB_PREFIX/qdirstub.h \
    $$SRC_PREFIX/themebusinesslogic.h \
    $$SRC_PREFIX/themedescriptor.h \
    $$SRC_PREFIX/themelistmodel.h \
    $$SRC_PREFIX/themecellcreator.h 

SOURCES += \
    $$STUB_PREFIX/mdesktopentry.cpp \
    ../../src/debug.cpp \
    ut_themecellcreator.cpp \
    $$SRC_PREFIX/themebusinesslogic.cpp \
    $$SRC_PREFIX/themedescriptor.cpp \
    $$SRC_PREFIX/themelistmodel.cpp \
    $$SRC_PREFIX/themecellcreator.cpp 

INSTALLS += target
