include(../coverage.pri)
include(../../localconfig.pri)

TEMPLATE = lib
CONFIG += plugin \
          gui \
          dbus \
          meegotouch \
          meegotouch_defines \
          duicontrolpanel \
          silent

MOC_DIR = .moc
OBJECTS_DIR = .objects

QT += dbus

load(meegotouch_defines)
DEFINES += \
    MEEGO_THEMEDIR=\\\"\"$$M_THEME_DIR\"\\\"

HEADERS = \
    ../debug.h \
    themedescriptor.h \
    themebusinesslogic.h \
    themeapplet.h \
    themelistmodel.h \
    themewidget.h \
    themebrief.h \
    themecellcreator.h \
    themedialog.h

SOURCES = \
    ../debug.cpp \
    themedescriptor.cpp \
    themebusinesslogic.cpp \
    themeapplet.cpp \
    themelistmodel.cpp \
    themewidget.cpp \
    themebrief.cpp \
    themecellcreator.cpp \
    themedialog.cpp

DESTDIR = lib
target.path += $$(DEBIAN_DESTDIR)$$[QT_INSTALL_LIBS]/duicontrolpanel/applets

desktop.files += *.desktop
desktop.path = $$(DEBIAN_DESTDIR)/usr/lib/duicontrolpanel

css.files = themeapplet.css
css.path += $$(DEBIAN_DESTDIR)/usr/share/themes/base/meegotouch/duicontrolpanel/style

rfs.files = theme-rfs.sh
rfs.path += $$(DEBIAN_DESTDIR)/$$RFS_DIR

message("The plugin will be installed to: " $$target.path)
message("CSS path will be: " $$css.path)
INSTALLS += target \
            css \
            rfs \
            desktop 

