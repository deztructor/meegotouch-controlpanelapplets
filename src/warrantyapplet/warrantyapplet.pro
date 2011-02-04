include(../coverage.pri)
include(../../localconfig.pri)

TEMPLATE = lib
CONFIG += plugin \
          gui \
          meegotouchcore \
          duicontrolpanel \

contains(DEFINES, HAVE_QMSYSTEM) {
    CONFIG += qmsystem2
}

QT += dbus

MOC_DIR = .moc
OBJECTS_DIR = .objects

HEADERS = \
    ../debug.h \
    warrantybusinesslogic.h \
    warrantyapplet.h \
    warrantywidget.h

SOURCES = \
    ../debug.cpp \
    warrantybusinesslogic.cpp \
    warrantyapplet.cpp \
    warrantywidget.cpp

DESTDIR = lib
desktop.files += *.desktop
desktop.path = $$(DEBIAN_DESTDIR)/usr/lib/duicontrolpanel
target.path += $$(DEBIAN_DESTDIR)$$[QT_INSTALL_LIBS]/duicontrolpanel/applets

css.files = libwarrantyapplet.css
css.path += $$(DEBIAN_DESTDIR)/usr/share/themes/base/meegotouch/libwarrantyapplet/style

INSTALLS +=   \
    target    \
    desktop   \
    css

