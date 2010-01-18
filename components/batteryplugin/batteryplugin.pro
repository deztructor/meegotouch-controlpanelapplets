MOC_DIR = .moc
OBJECTS_DIR = .objects
TEMPLATE = lib
LIBS += -lduicontrolpanel
CONFIG += plugin \
    gui \
    dui \
    duistatusindicatormenu
QT += dbus
INCLUDEPATH += \
    ../batteryapplet \
    /usr/include/duicontrolpanel
HEADERS = batteryplugin.h \
    battery.h \
    ../batteryapplet/batterydbusinterface.h \
    ../batteryapplet/batteryimage.h
SOURCES = batteryplugin.cpp \
    battery.cpp \
    ../batteryapplet/batterydbusinterface.cpp \
    ../batteryapplet/batteryimage.cpp
TARGET = $$qtLibraryTarget(battery)
DESTDIR = lib
target.path += /usr/lib/duistatusindicatormenu/plugins
css.files = batteryplugin.css
css.path += /usr/share/duistatusindicatormenu/themes/style/
INSTALLS += target \
    css