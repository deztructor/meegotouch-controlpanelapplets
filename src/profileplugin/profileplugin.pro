include(../../localconfig.pri)
include(../coverage.pri)

TEMPLATE = lib

MOC_DIR = .moc
MGEN_OUTDIR = .gen
OBJECTS_DIR = .obj

CONFIG += \
    plugin \
    gui \
    meegotouch \
    duicontrolpanel \
    system-ui \
    link_pkgconfig \
    silent

PKGCONFIG += \
    profile \
    dbus-1

CXXFLAGS += -DM_LIBRARY_NAME=libstatusindicatormenu-profile

QT += dbus
INCLUDEPATH  += ../profileapplet

HEADERS = profileplugin.h \
          profilewidget.h \
          profiledialog.h \
          ../profileapplet/profilebackend.h \
          ../profileapplet/profiledatainterface.h \
          ../debug.h
          
SOURCES = profileplugin.cpp \
          profilewidget.cpp \
          profiledialog.cpp \
          ../profileapplet/profilebackend.cpp \
          ../profileapplet/profiledatainterface.cpp \
          ../debug.cpp

desktop_entry.path = /usr/share/meegotouch/applicationextensions
desktop_entry.files = statusindicatormenu-profile.desktop

css.path = /usr/share/themes/base/meegotouch/libstatusindicatormenu-profile/style
css.files = libstatusindicatormenu-profile.css

TARGET        = $$qtLibraryTarget(statusindicatormenu-profile)
DESTDIR       = lib
target.path += /usr/lib/meegotouch/applicationextensions

INSTALLS += target \
            desktop_entry \
            css

