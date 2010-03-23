MOC_DIR = .moc
OBJECTS_DIR = .objects
TEMPLATE = lib

CONFIG += plugin \
          gui \
          dui \
          duistatusindicatormenu \
          silent

QT += dbus

INCLUDEPATH += \
    /usr/include/duistatusindicatormenu

HEADERS = volumebarplugin.h \
          volumebarlogic.h \
          volumebar.h \
          ../debug.h

SOURCES = volumebarplugin.cpp \
          volumebarlogic.cpp \
          volumebar.cpp \
          ../debug.cpp

TARGET = $$qtLibraryTarget(volume)
DESTDIR = lib
target.path += /usr/lib/duistatusindicatormenu/plugins

INSTALLS += target

