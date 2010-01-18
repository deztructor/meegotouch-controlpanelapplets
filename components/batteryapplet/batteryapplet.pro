TEMPLATE = lib
CONFIG += plugin \
    gui \
    dui \
    silent \
    debug
LIBS += -lduicontrolpanel
INCLUDEPATH += /usr/include/qt4/dui \
    /usr/include/duicontrolpanel
QT += dbus
contains(cov, true) { 
    message("Coverage options enabled")
    QMAKE_CXXFLAGS += --coverage
    QMAKE_LFLAGS += --coverage
}
MOC_DIR = .moc
OBJECTS_DIR = .objects
HEADERS = batteryapplet.h \
    batterywidget.h \
    batterybrief.h \
    batterydbusinterface.h \    
    timecontainer.h \
    slidercontainer.h \
    dcpbattery.h \
    batteryimage.h

SOURCES = batteryapplet.cpp \
    batterywidget.cpp \
    batterybrief.cpp \
    batterydbusinterface.cpp \    
    timecontainer.cpp \
    slidercontainer.cpp \
    batteryimage.cpp

css.files = batteryapplet.css
DESTDIR = lib
desktop.files += *.desktop
desktop.path = $$(DEBIAN_DESTDIR)/usr/lib/duicontrolpanel
target.path += $$(DEBIAN_DESTDIR)$$[QT_INSTALL_LIBS]/duicontrolpanel/applets
css.path += $$(DEBIAN_DESTDIR)/usr/share/duicontrolpanel/themes/style
message("The plugin will be installed to: " $$target.path)
message("CSS path will be: " $$css.path)
INSTALLS += target \
    css \
    desktop
OTHER_FILES += batteryapplet.css \
    battery.desktop