include(../common_top.pri)

MOC_DIR = .moc
SRC_PREFIX = ../../src/wallpaperapplet
STUB_PREFIX = ../stubs
INCLUDEPATH += \
	$$SRC_PREFIX \
        $$STUB_PREFIX

QT += \
    testlib \
    dbus 

TEMPLATE = app
DEFINES += UNIT_TEST
TARGET = ut_wallpaperdescriptor
target.path = /usr/lib/$$TEST_PKG_NAME

CONFIG += \
	plugin \
	gui \
	meegotouch \
	duicontrolpanel \
	silent 

HEADERS += \
    $$STUB_PREFIX/thumbnailerstub.h \
    $$STUB_PREFIX/trackerstub.h \
    ../../src/debug.h \
    ut_wallpaperdescriptor.h \
    $$SRC_PREFIX/wallpaperdescriptor.h 

SOURCES += \
    $$STUB_PREFIX/thumbnailerstub.cpp \
    $$STUB_PREFIX/trackerstub.cpp \
    ../../src/debug.cpp \
    ut_wallpaperdescriptor.cpp \
    $$SRC_PREFIX/wallpaperdescriptor.cpp 

INSTALLS += target


