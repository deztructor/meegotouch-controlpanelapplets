include(../../localconfig.pri)
include(../coverage.pri)

TEMPLATE = lib
VERSION = 0.1.0
MOC_DIR = .moc
TARGET = meegocontrol
OBJECTS_DIR = .objects

QT += dbus

CONFIG +=          \
    dll            \
    gui            \
    quill          \
    meegotouchcore \
    link_pkgconfig \
    duicontrolpanel

QMAKE_LFLAGS_RPATH = -Wl


#
# FIXME: These are needed for the soundssettingsapplet. We maybe could remove
# some of these, but for that we need to use the LIBMEEGOCONTROL define in the
# soundsettingsapplet source.
#
PKGCONFIG += dbus-1 profile gconf-2.0 ContentManager gstreamer-0.10

#########################################
# wallpaper setting extra dependencies  #
#########################################
contains(DEFINES, HAVE_CONTENT_MANAGER) {
    PKGCONFIG += ContentManager
}

contains(DEFINES, HAVE_QUILL_FILTER) {
    CONFIG += quillimagefilter
}

contains(DEFINES, HAVE_QUILL_METADATA) {
    CONFIG += quillmetadata
}

###########################################
# soundsetting applet extra dependencies  #
###########################################
contains(DEFINES, HAVE_LIBRESOURCEQT) {
    PKGCONFIG += libresourceqt1
}

#
# This way the code wil always know that it is running outside the controlpanel
#
DEFINES += \
    LIBMEEGOCONTROL

LIBS += \
    -lqttracker \
    -lthumbnailer

INCLUDEPATH += \
    /usr/include/QtTracker \
    /usr/include/thumbnailer \
    ../wallpaperapplet

WALLPAPER_PATH = ../wallpaperapplet
SOUNDSETTINGS_PATH = ../soundsettingsapplet

PUBLIC_HEADERS = \
    $$WALLPAPER_PATH/wallpaperbusinesslogic.h      \
    $$WALLPAPER_PATH/wallpaperdescriptor.h         \
    $$WALLPAPER_PATH/wallpapercurrentdescriptor.h  \
    $$WALLPAPER_PATH/wallpaperitrans.h             \
    $$WALLPAPER_PATH/wallpapereditorwidget.h       \
    $$WALLPAPER_PATH/wallpaperinfoheader.h         \
    $$SOUNDSETTINGS_PATH/static.h                  \
    $$SOUNDSETTINGS_PATH/alerttone.h               \
    $$SOUNDSETTINGS_PATH/alerttonedefaults.h       \
    $$SOUNDSETTINGS_PATH/alerttonetoplevel.h       \
    $$SOUNDSETTINGS_PATH/alerttonebrowser.h        \
    $$SOUNDSETTINGS_PATH/alerttoneappletmaps.h     \
    $$SOUNDSETTINGS_PATH/trackerconnection.h       \
    $$SOUNDSETTINGS_PATH/drilldownitem.h           \
    $$SOUNDSETTINGS_PATH/qprofilevalue.h           \
    $$SOUNDSETTINGS_PATH/qtrackedvariant.h         \
    $$SOUNDSETTINGS_PATH/alerttonedefaultsmodel.h  \
    WallpaperBusinessLogic                         \
    WallpaperDescriptor                            \
    WallpaperCurrentDescriptor                     \
    WallpaperITrans                                \
    WallpaperEditorWidget                          \
    init.h                                         \
    meegocontrol.h

HEADERS =                                          \
    ../debug.h                                     \
    $$SOUNDSETTINGS_PATH/alerttonepreview.h        \
    $$PUBLIC_HEADERS

SOURCES =                                          \
    ../debug.cpp                                   \
    init.c                                         \
    $$WALLPAPER_PATH/wallpaperitrans.cpp           \
    $$WALLPAPER_PATH/wallpaperdescriptor.cpp       \
    $$WALLPAPER_PATH/wallpapercurrentdescriptor.cpp \
    $$WALLPAPER_PATH/wallpaperbusinesslogic.cpp    \
    $$WALLPAPER_PATH/wallpapereditorwidget.cpp     \
    $$WALLPAPER_PATH/wallpaperinfoheader.cpp       \
    $$SOUNDSETTINGS_PATH/alerttone.cpp             \
    $$SOUNDSETTINGS_PATH/alerttonedefaults.cpp     \
    $$SOUNDSETTINGS_PATH/alerttonetoplevel.cpp     \
    $$SOUNDSETTINGS_PATH/alerttonebrowser.cpp      \
    $$SOUNDSETTINGS_PATH/alerttoneappletmaps.cpp   \
    $$SOUNDSETTINGS_PATH/trackerconnection.cpp     \
    $$SOUNDSETTINGS_PATH/drilldownitem.cpp         \
    $$SOUNDSETTINGS_PATH/qprofilevalue.cpp         \
    $$SOUNDSETTINGS_PATH/qtrackedvariant.cpp       \
    $$SOUNDSETTINGS_PATH/alerttonedefaultsmodel.cpp \
    $$SOUNDSETTINGS_PATH/alerttonepreview.cpp      

DESTDIR = lib
target.path += /usr/lib

headers.files = $$PUBLIC_HEADERS
headers.path += $$(DEBIAN_DESTDIR)/usr/include/libmeegocontrol

qtfeature.path = $$[QT_INSTALL_DATA]/mkspecs/features
qtfeature.files = \
    meegocontrol.prf \
    meegocontrol_sound.prf

# css.files = wallpaper.css
# css.path =  ??

INSTALLS += \
    qtfeature \
    headers \
    target

