include(../coverage.pri)
include(../check.pri)

MOC_DIR = .moc
SRC_PREFIX = ../../src/themeapplet
STUB_PREFIX = ../stubs
INCLUDEPATH += $$SRC_PREFIX \
               $$STUB_PREFIX
QT += \
    testlib \
    dbus 

TEMPLATE = app

DEFINES += \
	UNIT_TEST 

TARGET = ut_themeapplet
target.path = /usr/lib/systemui-applets-tests

CONFIG += \
	gui \
	meegotouch \
	plugin \
	duicontrolpanel \
	silent 

HEADERS += \
    ../../src/debug.h \
    ut_themeapplet.h \
    $$STUB_PREFIX/mdesktopentrystub.h \
    $$SRC_PREFIX/themeapplet.h \
    $$SRC_PREFIX/themebrief.h \
    $$SRC_PREFIX/themebusinesslogic.h \
    $$SRC_PREFIX/themecellcreator.h \
    $$SRC_PREFIX/themedescriptor.h \
    $$SRC_PREFIX/themedialog.h \
    $$SRC_PREFIX/themelistmodel.h \
    $$SRC_PREFIX/themewidget.h

SOURCES += \
    ../../src/debug.cpp \
    ut_themeapplet.cpp \
    $$STUB_PREFIX/mdesktopentrystub.cpp \
    $$SRC_PREFIX/themeapplet.cpp \
    $$SRC_PREFIX/themebrief.cpp \
    $$SRC_PREFIX/themebusinesslogic.cpp \
    $$SRC_PREFIX/themecellcreator.cpp \
    $$SRC_PREFIX/themedescriptor.cpp \
    $$SRC_PREFIX/themedialog.cpp \
    $$SRC_PREFIX/themelistmodel.cpp \
    $$SRC_PREFIX/themewidget.cpp

INSTALLS += target
