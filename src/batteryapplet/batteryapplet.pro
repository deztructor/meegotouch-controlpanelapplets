include(../coverage.pri)
include(../../localconfig.pri)

TEMPLATE = lib
CONFIG += plugin \
          gui \
          meegotouch \
          silent \
          duicontrolpanel

contains(DEFINES, HAVE_USERGUIDE) {
    # FIXME: this has to be removed if the functionality has merged into meegotouch
    CONFIG += userguide
}

contains(DEFINES, HAVE_QMSYSTEM) {
    CONFIG += qmsystem2
}

QT += dbus

contains(cov, true) {
    message("Coverage options enabled")
    QMAKE_CXXFLAGS += --coverage
    QMAKE_LFLAGS += --coverage
}

MOC_DIR = .moc
OBJECTS_DIR = .objects
HEADERS =                  \
    ../debug.h             \
    batteryapplet.h        \
    batterywidget.h        \
    batterybrief.h         \
    batterybusinesslogic.h \
    percentagecontainer.h  \
    slidercontainer.h      \
    dcpbattery.h           \
    batteryimage.h

SOURCES =                    \
    ../debug.cpp             \
    batteryapplet.cpp        \
    batterywidget.cpp        \
    batterybrief.cpp         \
    batterybusinesslogic.cpp \
    percentagecontainer.cpp  \
    slidercontainer.cpp      \
    batteryimage.cpp

DESTDIR = lib
target.path += $$(DEBIAN_DESTDIR)$$[QT_INSTALL_LIBS]/duicontrolpanel/applets

css.files = batteryapplet.css
css.path += $$(DEBIAN_DESTDIR)/usr/share/themes/base/meegotouch/duicontrolpanel/style

desktop.files += *.desktop
desktop.path = $$(DEBIAN_DESTDIR)/usr/lib/duicontrolpanel

rfs.files = battery-rfs.sh
rfs.path += $$(DEBIAN_DESTDIR)/$$RFS_DIR

message("The plugin will be installed to: " $$target.path)
message("CSS path will be: " $$css.path)
message("Restore script path: " $$rfs.path)

INSTALLS += target \
            css \
            rfs \
            desktop

