TEMPLATE      = lib
CONFIG       += plugin \
                gui \
                m \
                silent \
                debug
LIBS += -lmcontrolpanel
INCLUDEPATH += /usr/include/qt4/m \
               /usr/include/mcontrolpanel
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
desktop.path = $$(DEBIAN_DESTDIR)/usr/lib/mcontrolpanel
target.path +=  $$(DEBIAN_DESTDIR)$$[QT_INSTALL_LIBS]/mcontrolpanel/applets

message("The plugin will be installed to: " $$target.path)
INSTALLS += target \
            desktop

