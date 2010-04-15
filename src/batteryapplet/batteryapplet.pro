TEMPLATE = lib
CONFIG += plugin \
    gui \
    m \
    silent \
    debug \
    mcontrolpanel

LIBS += -lmcontrolpanel 

INCLUDEPATH += /usr/include/qt4/m \
               /usr/include/mcontrolpanel 

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
    batterydbusinterface.h \    
    timecontainer.h        \
    slidercontainer.h      \
    dcpbattery.h           \
    batteryimage.h

SOURCES =                  \
    ../debug.cpp           \
    batteryapplet.cpp \
    batterywidget.cpp \
    batterybrief.cpp \
    batterydbusinterface.cpp \    
    timecontainer.cpp \
    slidercontainer.cpp \
    batteryimage.cpp

css.files = batteryapplet.css
DESTDIR = lib
desktop.files += *.desktop
desktop.path = $$(DEBIAN_DESTDIR)/usr/lib/mcontrolpanel
target.path += $$(DEBIAN_DESTDIR)$$[QT_INSTALL_LIBS]/mcontrolpanel/applets
css.path += $$(DEBIAN_DESTDIR)/usr/share/themes/base/m/mcontrolpanel/style
message("The plugin will be installed to: " $$target.path)
message("CSS path will be: " $$css.path)
INSTALLS += target \
            css \
            desktop

OTHER_FILES += batteryapplet.css \
               battery.desktop

