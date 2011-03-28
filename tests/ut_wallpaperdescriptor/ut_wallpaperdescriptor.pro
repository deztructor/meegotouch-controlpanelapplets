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
    ../../src/debug.h \
    ut_wallpaperdescriptor.h \
    $$SRC_PREFIX/wallpaperdescriptor.h 

SOURCES += \
    $$STUB_PREFIX/thumbnailerstub.cpp \
    ../../src/debug.cpp \
    ut_wallpaperdescriptor.cpp \
    $$SRC_PREFIX/wallpaperdescriptor.cpp 

INSTALLS += target


