include(../coverage.pri)

TEMPLATE = lib

MOC_DIR = .moc
MGEN_OUTDIR = .gen
OBJECTS_DIR = .obj

CONFIG       += plugin \
                gui \
                meegotouch \
                duicontrolpanel \
                system-ui \
                silent

QT += dbus

LIBS += -lprofile-qt

INCLUDEPATH  += ../profileapplet \
                /usr/include/profile-qt

HEADERS = profileplugin.h \
          profilewidget.h \
          profilebuttons.h \
          ../profileapplet/profiledatainterface.h \
          ../debug.h
          
SOURCES = profileplugin.cpp \
          profilewidget.cpp \
          profilebuttons.cpp \
          ../profileapplet/profiledatainterface.cpp \
          ../debug.cpp

desktop_entry.path = /usr/share/meegotouch/applicationextensions
desktop_entry.files = statusindicatormenu-profile.desktop

TARGET        = $$qtLibraryTarget(statusindicatormenu-profile)
DESTDIR       = lib
target.path += /usr/lib/meegotouch/applicationextensions
INSTALLS +=  target \
        desktop_entry
