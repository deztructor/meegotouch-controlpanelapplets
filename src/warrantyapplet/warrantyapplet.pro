TEMPLATE = lib
CONFIG += plugin \
          gui \
          meegotouch \
          duicontrolpanel \
          silent

QT += dbus

contains(cov, true) { 
    message("Coverage options enabled")
    QMAKE_CXXFLAGS += --coverage
    QMAKE_LFLAGS += --coverage
}

MOC_DIR = .moc
OBJECTS_DIR = .objects

HEADERS = \
    ../debug.h \
    warrantybusinesslogic.h \
    warrantyapplet.h \
    warrantywidget.h \
    warrantybrief.h 

SOURCES = \
    ../debug.cpp \
    warrantybusinesslogic.cpp \
    warrantyapplet.cpp \
    warrantywidget.cpp \
    warrantybrief.cpp 

DESTDIR = lib
desktop.files += *.desktop
desktop.path = $$(DEBIAN_DESTDIR)/usr/lib/duicontrolpanel
target.path += $$(DEBIAN_DESTDIR)$$[QT_INSTALL_LIBS]/duicontrolpanel/applets

message("The plugin will be installed to: " $$target.path)
INSTALLS += target \
            desktop
