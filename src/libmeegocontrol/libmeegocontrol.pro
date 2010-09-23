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
HEADERS = \
    ../debug.h \
    $$WALLPAPER_PATH/wallpaperdescriptor.h \
    $$WALLPAPER_PATH/wallpapercurrentdescriptor.h \
    $$WALLPAPER_PATH/wallpapermodel.h \
    $$WALLPAPER_PATH/wallpaperlist.h \
    $$WALLPAPER_PATH/wallpaperbusinesslogic.h \
    $$WALLPAPER_PATH/wallpaperapplet.h \
    $$WALLPAPER_PATH/wallpaperwidget.h \
    $$WALLPAPER_PATH/wallpaperinfoheader.h \
    $$WALLPAPER_PATH/wallpapereditorwidget.h \
    $$WALLPAPER_PATH/wallpaperitrans.h \
    $$WALLPAPER_PATH/wallpaperbrief.h

SOURCES = \
    ../debug.cpp \
    $$WALLPAPER_PATH/wallpaperdescriptor.cpp \
    $$WALLPAPER_PATH/wallpapercurrentdescriptor.cpp \
    $$WALLPAPER_PATH/wallpaperlist.cpp \
    $$WALLPAPER_PATH/wallpapermodel.cpp \
    $$WALLPAPER_PATH/wallpaperbusinesslogic.cpp \
    $$WALLPAPER_PATH/wallpaperapplet.cpp \
    $$WALLPAPER_PATH/wallpaperwidget.cpp \
    $$WALLPAPER_PATH/wallpaperinfoheader.cpp \
    $$WALLPAPER_PATH/wallpapereditorwidget.cpp \
    $$WALLPAPER_PATH/wallpaperitrans.cpp \
    $$WALLPAPER_PATH/wallpaperbrief.cpp

DESTDIR = lib
target.path += /usr/lib

# TODO: FIXME
headers.files = \
    *.h
headers.path += $$(DEBIAN_DESTDIR)/usr/include/libmeegocontrol

# css.files = wallpaper.css
# css.path =  ??

INSTALLS += \
    headers \
    target


