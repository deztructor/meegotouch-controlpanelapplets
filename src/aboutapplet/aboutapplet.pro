include(../../localconfig.pri)
include(../coverage.pri)

TEMPLATE = lib
CONFIG += plugin \
          gui \
          meegotouchcore \
          mobility \
          duicontrolpanel \
          silent \
          link_pkgconfig

QT += dbus
MOBILITY += systeminfo

MOC_DIR = .moc
OBJECTS_DIR = .objects

QMAKE_LFLAGS_RPATH = -Wl

HEADERS =                   \
    ../debug.h              \
    aboutbusinesslogic.h    \
    aboutapplet.h           \
    aboutwidget.h

SOURCES =                   \
    ../debug.cpp            \
    aboutbusinesslogic.cpp  \
    aboutapplet.cpp         \
    aboutwidget.cpp

DESTDIR = lib
desktop.files += *.desktop
desktop.path = $$(DEBIAN_DESTDIR)/usr/lib/duicontrolpanel

css.files = libaboutapplet.css
css.path += $$(DEBIAN_DESTDIR)/usr/share/themes/base/meegotouch/libaboutapplet/style

target.path += $$(DEBIAN_DESTDIR)$$[QT_INSTALL_LIBS]/duicontrolpanel/applets

INSTALLS += target  \
            desktop \
            css

