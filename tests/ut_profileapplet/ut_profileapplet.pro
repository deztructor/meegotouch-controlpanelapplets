include(../common_top.pri)

MOC_DIR = .moc
SRC_PREFIX = ../../src/profileapplet
STUB_PREFIX = ../stubs
INCLUDEPATH += $$SRC_PREFIX \
               $$STUB_PREFIX
QT += \
    testlib \
    dbus 

TEMPLATE = app

DEFINES += \
	UNIT_TEST 

TARGET = ut_profileapplet
target.path = /usr/lib/$$TEST_PKG_NAME

CONFIG += \
	gui \
	meegotouch \
	plugin \
	duicontrolpanel \
	silent 

HEADERS += \
    $$STUB_PREFIX/profilestub.h \
    ../../src/debug.h \
    ut_profileapplet.h \
    $$SRC_PREFIX/dcpprofile.h \
    $$SRC_PREFIX/profileapplet.h \
    $$SRC_PREFIX/profilecontainer.h \
    $$SRC_PREFIX/profiledatainterface.h \
    $$SRC_PREFIX/profilewidget.h

SOURCES += \
    $$STUB_PREFIX/profilestub.cpp \
    ../../src/debug.cpp \
    ut_profileapplet.cpp \
    $$SRC_PREFIX/profileapplet.cpp \
    $$SRC_PREFIX/profilecontainer.cpp \
    $$SRC_PREFIX/profiledatainterface.cpp \
    $$SRC_PREFIX/profilewidget.cpp

INSTALLS += target
