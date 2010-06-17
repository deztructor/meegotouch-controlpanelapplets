include(../coverage.pri)

TEMPLATE      = lib

CONFIG       += plugin \
                gui \
                meegotouch \
                duicontrolpanel \
                qmsystem \
                silent

QT += gui dbus

MOC_DIR	      = .moc
OBJECTS_DIR   = .objects

HEADERS +=  \
    ../debug.h \
    usbapplet.h \
    usbbrief.h \
    usbview.h

SOURCES += \
    ../debug.cpp \
    usbapplet.cpp \
    usbbrief.cpp \
    usbview.cpp

DESTDIR = lib
desktop.files += usbapplet.desktop
desktop.path = $$(DEBIAN_DESTDIR)/usr/lib/duicontrolpanel
target.path +=  $$(DEBIAN_DESTDIR)$$[QT_INSTALL_LIBS]/duicontrolpanel/applets

message("The plugin will be installed to: " $$target.path)
INSTALLS += target \
            desktop

