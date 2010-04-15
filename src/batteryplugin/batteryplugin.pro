MOC_DIR = .moc
OBJECTS_DIR = .objects
TEMPLATE = lib
LIBS += -lmcontrolpanel
CONFIG += plugin \
          gui \
          m \
          mstatusindicatormenu \
          silent

QT += dbus
INCLUDEPATH += ../batteryapplet \
               /usr/include/mcontrolpanel \
               /usr/include/mstatusindicatormenu

HEADERS = batteryplugin.h \
          battery.h \
          ../batteryapplet/batterydbusinterface.h \
          ../batteryapplet/batteryimage.h \
          ../debug.h

SOURCES = batteryplugin.cpp \
          battery.cpp \
          ../batteryapplet/batterydbusinterface.cpp \
          ../batteryapplet/batteryimage.cpp \
          ../debug.cpp

TARGET = $$qtLibraryTarget(battery)
DESTDIR = lib
target.path += /usr/lib/mstatusindicatormenu/plugins

css.files = batteryplugin.css
css.path += /usr/share/mstatusindicatormenu/themes/style/

INSTALLS += target \
            css
