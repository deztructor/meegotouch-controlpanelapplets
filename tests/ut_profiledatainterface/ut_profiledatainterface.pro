SRC_PREFIX = ../../src/profileapplet
STUB_PREFIX = ../stubs
INCLUDEPATH += $$SRC_PREFIX \
               $$STUB_PREFIX

#
# Surely we can do better than this!
#
LIBS += -lprofile-qt
INCLUDEPATH += \
    /usr/include/duicontrolpanel \
    /usr/include/profile-qt

QT += \
    testlib \
    dbus 

TEMPLATE = app
DEFINES += UNIT_TEST
TARGET = ut_profiledatainterface
target.path = /usr/lib/systemui-applets-tests

CONFIG += \
	plugin \
	gui \
	meegotouch \
	duicontrolpanel \
	debug \
	silent 

HEADERS += \
    ../../src/debug.h \
    ut_profiledatainterface.h \
    $$SRC_PREFIX/dcpprofile.h \
    $$SRC_PREFIX/profileapplet.h \
    $$SRC_PREFIX/profilebrief.h \
    $$SRC_PREFIX/profilebuttons.h \
    $$SRC_PREFIX/profilecontainer.h \
    $$SRC_PREFIX/profiledatainterface.h \
    $$SRC_PREFIX/profilewidget.h

SOURCES += \
    ../../src/debug.cpp \
    ut_profiledatainterface.cpp \
    $$SRC_PREFIX/profileapplet.cpp \
    $$SRC_PREFIX/profilebrief.cpp \
    $$SRC_PREFIX/profilebuttons.cpp \
    $$SRC_PREFIX/profilecontainer.cpp \
    $$SRC_PREFIX/profiledatainterface.cpp \
    $$SRC_PREFIX/profilewidget.cpp

INSTALLS += target
