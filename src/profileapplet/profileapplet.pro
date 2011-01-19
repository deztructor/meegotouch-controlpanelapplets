include(../../localconfig.pri)
include(../coverage.pri)

TEMPLATE = lib
CONFIG += plugin \
          gui \
          meegotouchcore \
          duicontrolpanel \
          link_pkgconfig \
          silent

contains(DEFINES, HAVE_LIBPROFILE) {
    PKGCONFIG += profile dbus-1
}

QT += dbus
MOC_DIR = .moc
OBJECTS_DIR = .objects

HEADERS = \
    ../debug.h \
    profileapplet.h \
    profilewidget.h \
    profilecontainer.h \
    profilebackend.h \
    profiledatainterface.h

SOURCES = \
    ../debug.cpp \
    profileapplet.cpp \
    profilewidget.cpp \
    profilecontainer.cpp \
    profilebackend.cpp \
    profiledatainterface.cpp

DESTDIR = lib
target.path += $$(DEBIAN_DESTDIR)$$[QT_INSTALL_LIBS]/duicontrolpanel/applets

desktop.files += *.desktop
desktop.path = $$(DEBIAN_DESTDIR)/usr/lib/duicontrolpanel

INSTALLS += target \
            desktop

