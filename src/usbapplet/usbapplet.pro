TEMPLATE      = lib
CONFIG       += plugin \
                gui \
                meegotouch \
                duicontrolpanel \
                silent \
                link_pkgconfig \
                debug

PKGCONFIG += dbus-1 usb-moded
QT += dbus

contains(cov, true) { 
    message("Coverage options enabled")
    QMAKE_CXXFLAGS += --coverage
    QMAKE_LFLAGS += --coverage
}

MOC_DIR	      = .moc
OBJECTS_DIR   = .objects

HEADERS += usbapplet.h \
           usbbrief.h \
           usbmodes.h \
           usbview.h
SOURCES += usbapplet.cpp \
           usbbrief.cpp \
           usbmodes.cpp \
           usbview.cpp

DESTDIR = lib
desktop.files += usbapplet.desktop
desktop.path = $$(DEBIAN_DESTDIR)/usr/lib/duicontrolpanel
target.path +=  $$(DEBIAN_DESTDIR)$$[QT_INSTALL_LIBS]/duicontrolpanel/applets

message("The plugin will be installed to: " $$target.path)
INSTALLS += target \
            desktop

