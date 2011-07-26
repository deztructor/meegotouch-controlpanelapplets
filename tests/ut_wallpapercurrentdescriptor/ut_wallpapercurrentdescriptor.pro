include(../common_top.pri)

SRC_PREFIX = ../../src/wallpaperapplet
STUB_PREFIX = ../stubs

INCLUDEPATH = \
        $$STUB_PREFIX \
        $$SRC_PREFIX \
        $$INCLUDEPATH

QT += \
    testlib \
    dbus 

TEMPLATE = app
DEFINES += UNIT_TEST
TARGET = ut_wallpapercurrentdescriptor
target.path = /usr/lib/$$TEST_PKG_NAME

CONFIG += \
	plugin \
	gui \
    quill \
	meegotouchcore \
	duicontrolpanel

contains(DEFINES, HAVE_QUILL_FILTER) {
    CONFIG += quillimagefilter
}

contains(DEFINES, HAVE_QUILL_METADATA) {
    CONFIG += quillmetadata
}

HEADERS += \
    $$STUB_PREFIX/thumbnailerstub.h \
    $$STUB_PREFIX/mdesktopentry.h \
    ../../src/debug.h \
    ut_wallpapercurrentdescriptor.h \
    $$SRC_PREFIX/wallpaperitrans.h \
    $$SRC_PREFIX/wallpaperdescriptor.h \
    $$SRC_PREFIX/wallpapercurrentdescriptor.h \
    $$SRC_PREFIX/wallpaperthumb.h

SOURCES += \
    $$STUB_PREFIX/thumbnailerstub.cpp \
    $$STUB_PREFIX/mdesktopentry.cpp \
    ../../src/debug.cpp \
    ut_wallpapercurrentdescriptor.cpp \
    $$SRC_PREFIX/wallpaperitrans.cpp \
    $$SRC_PREFIX/wallpaperdescriptor.cpp \
    $$SRC_PREFIX/wallpapercurrentdescriptor.cpp \
    $$SRC_PREFIX/wallpaperthumb.cpp

INSTALLS += target

