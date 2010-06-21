include(../coverage.pri)
include(../check.pri)

SRC_PREFIX = ../../src/wallpaperapplet
STUB_PREFIX = ../stubs

INCLUDEPATH += \
	$$SRC_PREFIX \
        $$STUB_PREFIX

QT += \
    	testlib \
	dbus 

TEMPLATE = app

DEFINES += \
	UNIT_TEST 

TARGET = ut_wallpaperwidget
target.path = /usr/lib/systemui-applets-tests

CONFIG += \
	gui \
	meegotouch \
	plugin \
	duicontrolpanel \
	silent 

HEADERS += \
    $$STUB_PREFIX/thumbnailerstub.h \
    $$STUB_PREFIX/trackerstub.h \
    $$STUB_PREFIX/mdesktopentrystub.h \
    ../../src/debug.h \
    ut_wallpaperwidget.h \
    $$SRC_PREFIX/wallpaperbusinesslogic.h \
    $$SRC_PREFIX/wallpapercurrentdescriptor.h \
    $$SRC_PREFIX/wallpaperdescriptor.h \
    $$SRC_PREFIX/wallpaperitrans.h \
    $$SRC_PREFIX/wallpapermodel.h \ 
    $$SRC_PREFIX/wallpaperlist.h \
    $$SRC_PREFIX/wallpaperwidget.h 

SOURCES += \
    $$STUB_PREFIX/thumbnailerstub.cpp \
    $$STUB_PREFIX/trackerstub.cpp \
    $$STUB_PREFIX/mdesktopentrystub.cpp \
    ../../src/debug.cpp \
    ut_wallpaperwidget.cpp \
    $$SRC_PREFIX/wallpaperbusinesslogic.cpp \
    $$SRC_PREFIX/wallpapercurrentdescriptor.cpp \
    $$SRC_PREFIX/wallpaperdescriptor.cpp \
    $$SRC_PREFIX/wallpaperitrans.cpp \
    $$SRC_PREFIX/wallpapermodel.cpp \ 
    $$SRC_PREFIX/wallpaperlist.cpp \ 
    $$SRC_PREFIX/wallpaperwidget.cpp 

INSTALLS += target
MOC_DIR = .moc
