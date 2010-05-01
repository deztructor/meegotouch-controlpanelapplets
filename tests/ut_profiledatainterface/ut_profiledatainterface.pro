include(../coverage.pri)
SRC_PREFIX = ../../src/profileapplet
STUB_PREFIX = ../stubs
INCLUDEPATH += $$SRC_PREFIX \
               $$STUB_PREFIX

#
# Surely we can do better than this!
#
LIBS += -lprofile-qt
INCLUDEPATH += \
    /usr/include/profile-qt

QT += \
    testlib \
    dbus 

TEMPLATE = app
DEFINES += UNIT_TEST
TARGET = ut_profiledatainterface
target.path = /usr/lib/systemui-applets-tests

CONFIG += \
	gui \
	meegotouch \
	debug \
	silent 

HEADERS += \
    ../../src/debug.h \
    ut_profiledatainterface.h \
    $$SRC_PREFIX/profiledatainterface.h

SOURCES += \
    ../../src/debug.cpp \
    ut_profiledatainterface.cpp \
    $$SRC_PREFIX/profiledatainterface.cpp

INSTALLS += target
