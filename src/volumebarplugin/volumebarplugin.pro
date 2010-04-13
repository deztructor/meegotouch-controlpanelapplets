MOC_DIR = .moc
OBJECTS_DIR = .objects
TEMPLATE = lib

CONFIG += plugin \
          gui \
          dui \
          duistatusindicatormenu \
          link_pkgconfig \
          qmsystem \
          silent

PKGCONFIG += dbus-1

LIBS += -ldbus-qeventloop

INCLUDEPATH += \
    /usr/include/duistatusindicatormenu \
    /usr/include/dbus-1.0

HEADERS = volumebarplugin.h \
          volumebarlogic.h \
          volumeoverlay.h \
          volumebar.h \
          ../debug.h

SOURCES = volumebarplugin.cpp \
          volumebarlogic.cpp \
          volumeoverlay.cpp \
          volumebar.cpp \
          ../debug.cpp

TARGET = $$qtLibraryTarget(volume)
DESTDIR = lib
target.path += /usr/lib/duistatusindicatormenu/plugins

css.files += volumebar.css
css.path += $$(DEBIAN_DESTDIR)/usr/share/themes/base/dui/sysuid

INSTALLS += \
    target \
    css

