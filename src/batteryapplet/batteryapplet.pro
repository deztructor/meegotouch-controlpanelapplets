include(../coverage.pri)

TEMPLATE = lib
CONFIG += plugin \
          gui \
          meegotouch \
          qmsystem \
          silent \
          duicontrolpanel

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
rfsscript.files = battery-rfs.sh
rfsscript.path = $$system(/usr/bin/pkg-config --variable osso_rfs_scripts_dir clean-device)
target.path += $$(DEBIAN_DESTDIR)$$[QT_INSTALL_LIBS]/duicontrolpanel/applets
css.path += $$(DEBIAN_DESTDIR)/usr/share/themes/base/meegotouch/duicontrolpanel/style
message("The plugin will be installed to: " $$target.path)
message("CSS path will be: " $$css.path)
INSTALLS += target \
            rfsscript \
            css \
            desktop

OTHER_FILES += batteryapplet.css \
               battery-rfs.sh \
               battery.desktop



