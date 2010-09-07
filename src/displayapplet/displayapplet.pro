include(../coverage.pri)
include(../../localconfig.pri)

TEMPLATE = lib
CONFIG += plugin \
          gui \
          meegotouch \
          duicontrolpanel \
	  silent

contains(DEFINES, HAVE_QMSYSTEM) {
    CONFIG += qmsystem
}

QT += dbus gui

MOC_DIR = .moc
OBJECTS_DIR = .objects

HEADERS = \
    ../debug.h \
    displayapplet.h \
    displaywidget.h \
    displaybrief.h \
    displaybusinesslogic.h

SOURCES = \
    ../debug.cpp \
    displayapplet.cpp \
    displaywidget.cpp \
    displaybrief.cpp \
    displaybusinesslogic.cpp

DESTDIR = lib
desktop.files += *.desktop
desktop.path = $$(DEBIAN_DESTDIR)/usr/lib/duicontrolpanel
rfsscript.files = display-rfs.sh
rfsscript.path = $$system(/usr/bin/pkg-config --variable osso_rfs_scripts_dir clean-device)
target.path += $$(DEBIAN_DESTDIR)$$[QT_INSTALL_LIBS]/duicontrolpanel/applets
css.files = display.css
css.path = \
  $$(DEBIAN_DESTDIR)/usr/share/themes/base/meegotouch/duicontrolpanel/style

message("The plugin will be installed to: " $$target.path)

INSTALLS += target \
            desktop \
            rfsscript \
            css
