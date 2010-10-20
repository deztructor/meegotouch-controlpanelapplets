include(../coverage.pri)
include(../../localconfig.pri)

TEMPLATE = lib
CONFIG += plugin \
          gui \
          meegotouch \
          silent \
          duicontrolpanel

# FIXME: this has to be removed if the functionality has merged into meegotouch
INCLUDEPATH += /usr/include/userguide
DEPENDPATH += /usr/include/userguide
LIBS += -luserguide

contains(DEFINES, HAVE_QMSYSTEM) {
    CONFIG += qmsystem
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

css.files = batteryapplet.css
DESTDIR = lib
desktop.files += *.desktop
desktop.path = $$(DEBIAN_DESTDIR)/usr/lib/duicontrolpanel
target.path += $$(DEBIAN_DESTDIR)$$[QT_INSTALL_LIBS]/duicontrolpanel/applets
css.path += $$(DEBIAN_DESTDIR)/usr/share/themes/base/meegotouch/duicontrolpanel/style
message("The plugin will be installed to: " $$target.path)
message("CSS path will be: " $$css.path)
INSTALLS += target \
            css \
            desktop

OTHER_FILES += batteryapplet.css \
               battery.desktop



