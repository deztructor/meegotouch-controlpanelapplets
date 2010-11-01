include(../coverage.pri)
include(../../localconfig.pri)

TEMPLATE      = lib

CONFIG       += plugin \
                gui \
                meegotouch \
                duicontrolpanel \
                silent

contains(DEFINES, HAVE_QMSYSTEM) {
    CONFIG += qmsystem2
}

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
target.path +=  $$(DEBIAN_DESTDIR)$$[QT_INSTALL_LIBS]/duicontrolpanel/applets

desktop.files += usbapplet.desktop
desktop.path = $$(DEBIAN_DESTDIR)/usr/lib/duicontrolpanel

rfs.files = usb-rfs.sh
rfs.path += $$(DEBIAN_DESTDIR)/$$RFS_DIR

message("The plugin will be installed to: " $$target.path)
INSTALLS += target \
            rfs \
            desktop

