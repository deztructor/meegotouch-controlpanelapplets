TEMPLATE      = lib
CONFIG       += plugin \
                gui \
                meegotouch \
                duicontrolpanel \
                link_pkgconfig \
                silent \
                debug

PKGCONFIG += dbus-1 usb_moded
QT += dbus

# FIXME: HACK!!! REMOVE THIS!!!
message("HACK: fixing usb-moded headers" $$system(cp -f /usr/include/usb-moded/* $$PWD))

contains(cov, true) { 
    message("Coverage options enabled")
    QMAKE_CXXFLAGS += --coverage
    QMAKE_LFLAGS += --coverage
}

MOC_DIR	      = .moc
OBJECTS_DIR   = .objects

HEADERS +=  \
    ../debug.h \
    usbbusinesslogic.h \
    usbapplet.h \
    usbbrief.h \
    usbview.h

SOURCES += \
    ../debug.cpp \
    usbbusinesslogic.cpp \
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

