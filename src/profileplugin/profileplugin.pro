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
          ../profileapplet/profiledatainterface.h \
          ../profileapplet/profilebuttons.h \
          ../debug.h
          
SOURCES = profileplugin.cpp \
          profilewidget.cpp \
          ../profileapplet/profiledatainterface.cpp \
          ../profileapplet/profilebuttons.cpp \
          ../debug.cpp

TARGET        = $$qtLibraryTarget(profile)
DESTDIR       = lib
target.path += /usr/lib/meegotouch/statusindicatormenuplugins
INSTALLS +=  target
