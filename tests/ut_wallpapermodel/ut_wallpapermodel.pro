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

DEFINES += \
	UNIT_TEST 

TARGET = ut_wallpapermodel
target.path = /usr/lib/$$TEST_PKG_NAME

CONFIG += \
	gui \
	meegotouch \
	plugin \
	duicontrolpanel \
	silent 

HEADERS += \
    $$STUB_PREFIX/thumbnailerstub.h \
    $$STUB_PREFIX/trackerstub.h \
    $$STUB_PREFIX/mdesktopentry.h \
    $$STUB_PREFIX/filesystemstub.h \
    ../../src/debug.h \
    ut_wallpapermodel.h \
    $$SRC_PREFIX/gridimagewidget.h \
    $$SRC_PREFIX/wallpapergconf.h \
    $$SRC_PREFIX/wallpaperbusinesslogic.h \
    $$SRC_PREFIX/wallpapercurrentdescriptor.h \
    $$SRC_PREFIX/wallpaperdescriptor.h \
    $$SRC_PREFIX/wallpaperitrans.h \
    $$SRC_PREFIX/wallpapermodel.h 

SOURCES += \
    $$STUB_PREFIX/thumbnailerstub.cpp \
    $$STUB_PREFIX/trackerstub.cpp \
    $$STUB_PREFIX/mdesktopentry.cpp \
    $$STUB_PREFIX/filesystemstub.cpp \
    ../../src/debug.cpp \
    ut_wallpapermodel.cpp \
    $$SRC_PREFIX/gridimagewidget.cpp \
    $$SRC_PREFIX/wallpaperbusinesslogic.cpp \
    $$SRC_PREFIX/wallpapercurrentdescriptor.cpp \
    $$SRC_PREFIX/wallpaperdescriptor.cpp \
    $$SRC_PREFIX/wallpaperitrans.cpp \
    $$SRC_PREFIX/wallpapermodel.cpp 

INSTALLS += target
MOC_DIR = .moc
