MOC_DIR = .moc
OBJECTS_DIR = .objects
TEMPLATE = lib

CONFIG += plugin \
          gui \
          dui \
          duistatusindicatormenu \
          link_pkgconfig \
          silent

PKGCONFIG += dbus-1

LIBS += -ldbus-qeventloop

INCLUDEPATH += \
    /usr/include/duistatusindicatormenu \
    /usr/include/dbus-1.0

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

