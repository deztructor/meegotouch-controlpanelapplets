MOC_DIR = .moc
MGEN_OUTDIR = .gen
OBJECTS_DIR = .obj
QT += dbus
LIBS += -lprofile-qt \
        -lmcontrolpanel
TEMPLATE      = lib
CONFIG       += plugin gui m mstatusindicatormenu silent
INCLUDEPATH  += ../profileapplet \
                /usr/include/profile-qt \
                /usr/include/mcontrolpanel \
                /usr/include/mstatusindicatormenu

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
target.path += /usr/lib/mstatusindicatormenu/plugins/ 
INSTALLS +=  target
