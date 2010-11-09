include(../../localconfig.pri)
include(../coverage.pri)

TEMPLATE = lib
CONFIG += plugin \
          gui \
          meegotouch \
          duicontrolpanel \
          silent

contains(DEFINES, HAVE_PROFILE_QT) {
    LIBS += -lprofile-qt
    INCLUDEPATH += /usr/include/profile-qt
}

QT += dbus
MOC_DIR = .moc
OBJECTS_DIR = .objects
HEADERS = \
    ../debug.h \
    profileapplet.h \
    profilewidget.h \
    profilecontainer.h \
    profiledatainterface.h
SOURCES = \
    ../debug.cpp \
    profileapplet.cpp \
    profilewidget.cpp \
    profilecontainer.cpp \
    profiledatainterface.cpp

DESTDIR = lib
target.path += $$(DEBIAN_DESTDIR)$$[QT_INSTALL_LIBS]/duicontrolpanel/applets

desktop.files += *.desktop
desktop.path = $$(DEBIAN_DESTDIR)/usr/lib/duicontrolpanel

message("The plugin will be installed to: " $$target.path)
INSTALLS += target \
            desktop
