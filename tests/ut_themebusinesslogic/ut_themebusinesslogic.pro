SRC_PREFIX = ../../src/themeapplet
STUB_PREFIX = ../stubs
INCLUDEPATH += $$SRC_PREFIX \
               $$STUB_PREFIX

QT += \
    testlib \
    dbus 

TEMPLATE = app
DEFINES += UNIT_TEST
TARGET = ut_themebusinesslogic
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
    ut_themebusinesslogic.h \
    $$SRC_PREFIX/themeapplet.h \
    $$SRC_PREFIX/themebrief.h \
    $$SRC_PREFIX/themebusinesslogic.h \
    $$SRC_PREFIX/themecontentitem.h \
    $$SRC_PREFIX/themedescriptor.h \
    $$SRC_PREFIX/themedialog.h \
    $$SRC_PREFIX/themewidget.h

SOURCES += \
    ../../src/debug.cpp \
    ut_themebusinesslogic.cpp \
    $$SRC_PREFIX/themeapplet.cpp \
    $$SRC_PREFIX/themebrief.cpp \
    $$SRC_PREFIX/themebusinesslogic.cpp \
    $$SRC_PREFIX/themecontentitem.cpp \
    $$SRC_PREFIX/themedescriptor.cpp \
    $$SRC_PREFIX/themedialog.cpp \
    $$SRC_PREFIX/themewidget.cpp 

INSTALLS += target
