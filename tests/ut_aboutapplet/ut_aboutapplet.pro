include(../common_top.pri)

MOC_DIR = .moc
SRC_PREFIX = ../../src/aboutapplet
STUB_PREFIX = ../stubs

INCLUDEPATH = \
    $$STUB_PREFIX \
    $$SRC_PREFIX \
    $$INCLUDEPATH

QT += \
    testlib \
    dbus 

TEMPLATE = app

DEFINES += \
    UNIT_TEST 

TARGET = ut_aboutapplet
target.path = /usr/lib/$$TEST_PKG_NAME

CONFIG += \
    gui \
    meegotouch \
    mobility \
    plugin \
    duicontrolpanel \
    link_pkgconfig \
    silent 

MOBILITY += systeminfo

HEADERS += \
    $$STUB_PREFIX/MLabel \
    ../../src/debug.h \
    ut_aboutapplet.h \
    $$SRC_PREFIX/aboutapplet.h \
    $$SRC_PREFIX/aboutbrief.h \
    $$SRC_PREFIX/aboutbusinesslogic.h \
    $$SRC_PREFIX/aboutwidget.h

SOURCES += \
    $$STUB_PREFIX/mlabel_stub.cpp \
    ../../src/debug.cpp \
    ut_aboutapplet.cpp \
    $$SRC_PREFIX/aboutapplet.cpp \
    $$SRC_PREFIX/aboutbrief.cpp \
    $$SRC_PREFIX/aboutbusinesslogic.cpp \
    $$SRC_PREFIX/aboutwidget.cpp

INSTALLS += target
