include(../../localconfig.pri)
include(../coverage.pri)

TEMPLATE = lib
CONFIG += plugin \
          gui \
          meegotouch \
          mobility \
          duicontrolpanel \
          silent \
          link_pkgconfig

QT += dbus
MOBILITY += systeminfo

MOC_DIR = .moc
OBJECTS_DIR = .objects

HEADERS = \
    ../debug.h \
    aboutbusinesslogic.h \
    aboutapplet.h \
    aboutwidget.h \
    aboutbrief.h 

SOURCES = \
    ../debug.cpp \
    aboutbusinesslogic.cpp \
    aboutapplet.cpp \
    aboutwidget.cpp \
    aboutbrief.cpp 

DESTDIR = lib
desktop.files += *.desktop
desktop.path = $$(DEBIAN_DESTDIR)/usr/lib/duicontrolpanel
target.path += $$(DEBIAN_DESTDIR)$$[QT_INSTALL_LIBS]/duicontrolpanel/applets

message("The plugin will be installed to: " $$target.path)
INSTALLS += target \
            desktop
