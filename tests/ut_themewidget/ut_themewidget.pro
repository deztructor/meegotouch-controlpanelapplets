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
	plugin \
	duicontrolpanel \
	silent 

QT += \
    testlib \
    gui \
    dbus 

TEMPLATE = app
DEFINES += UNIT_TEST
TARGET = ut_themewidget
target.path = /usr/lib/systemui-applets-tests

HEADERS += \
    ../../src/debug.h \
    ut_themewidget.h \
    $$STUB_PREFIX/mdesktopentrystub.h \
    $$SRC_PREFIX/themebusinesslogic.h \
    $$SRC_PREFIX/themedescriptor.h \
    $$SRC_PREFIX/themelistmodel.h \
    $$SRC_PREFIX/themecellcreator.h \
    $$SRC_PREFIX/themedialog.h

SOURCES += \
    ../../src/debug.cpp \
    ut_themewidget.cpp \
    $$STUB_PREFIX/mdesktopentrystub.cpp \
    $$SRC_PREFIX/themebusinesslogic.cpp \
    $$SRC_PREFIX/themedescriptor.cpp \
    $$SRC_PREFIX/themelistmodel.cpp \
    $$SRC_PREFIX/themecellcreator.cpp \
    $$SRC_PREFIX/themedialog.cpp

INSTALLS += target
