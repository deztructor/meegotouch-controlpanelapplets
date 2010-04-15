TEMPLATE = lib
CONFIG += plugin \
    gui \
    m \
    silent \
    debug \
    qmsystem
LIBS += -lmcontrolpanel
INCLUDEPATH += /usr/include/qt4/m \
    /usr/include/mcontrolpanel
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
    displayapplet.h \
    displaywidget.h \
    displaybrief.h \
    displaybusinesslogic.h

SOURCES = \
    ../debug.cpp \
    displayapplet.cpp \
    displaywidget.cpp \
    displaybrief.cpp \
    displaybusinesslogic.cpp

DESTDIR = lib
desktop.files += *.desktop
desktop.path = $$(DEBIAN_DESTDIR)/usr/lib/mcontrolpanel
target.path += $$(DEBIAN_DESTDIR)$$[QT_INSTALL_LIBS]/mcontrolpanel/applets

message("The plugin will be installed to: " $$target.path)
INSTALLS += target \
    desktop
