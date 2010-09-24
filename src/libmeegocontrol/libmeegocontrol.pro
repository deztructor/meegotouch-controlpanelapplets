include(../../localconfig.pri)
include(../coverage.pri)

TEMPLATE = lib
MOC_DIR = .moc
TARGET = meegocontrol
OBJECTS_DIR = .objects

QT += dbus

CONFIG += plugin \
          gui \
          meegotouch \
          silent \
          duicontrolpanel
# TODO: ^ remove duicontrolpanel

#########################################
# wallpaper setting extra dependencies  #
#########################################
contains(DEFINES, HAVE_CONTENT_MANAGER) {
    CONFIG += link_pkgconfig
    PKGCONFIG += ContentManager
}

LIBS += \
    -lqttracker \
    -lthumbnailer

INCLUDEPATH += \
    /usr/include/QtTracker \
    /usr/include/thumbnailer \
    ../wallpaperapplet

WALLPAPER_PATH = ../wallpaperapplet

# TODO: FIXME: updatethis
PUBLIC_HEADERS = \
    $$WALLPAPER_PATH/wallpaperbusinesslogic.h \
    $$WALLPAPER_PATH/wallpaperdescriptor.h \
    $$WALLPAPER_PATH/wallpapercurrentdescriptor.h \
    $$WALLPAPER_PATH/wallpaperitrans.h \
    WallpaperBusinessLogic \
    WallpaperDescriptor \
    WallpaperCurrentDescriptor \
    WallpaperITrans \
    meegocontrol.h

HEADERS = \
    ../debug.h \
    $$PUBLIC_HEADERS \
    $$WALLPAPER_PATH/wallpaperitrans.h \
    $$WALLPAPER_PATH/wallpaperdescriptor.h \
    $$WALLPAPER_PATH/wallpapercurrentdescriptor.h \
    $$WALLPAPER_PATH/wallpaperbusinesslogic.h 

SOURCES = \
    ../debug.cpp \
    $$WALLPAPER_PATH/wallpaperitrans.cpp \
    $$WALLPAPER_PATH/wallpaperdescriptor.cpp \
    $$WALLPAPER_PATH/wallpapercurrentdescriptor.cpp \
    $$WALLPAPER_PATH/wallpaperbusinesslogic.cpp 

DESTDIR = lib
target.path += /usr/lib

headers.files = $$PUBLIC_HEADERS
headers.path += $$(DEBIAN_DESTDIR)/usr/include/libmeegocontrol

qtfeature.path = $$[QT_INSTALL_DATA]/mkspecs/features
qtfeature.files = meegocontrol.prf

# css.files = wallpaper.css
# css.path =  ??

INSTALLS += \
    qtfeature \
    headers \
    target

