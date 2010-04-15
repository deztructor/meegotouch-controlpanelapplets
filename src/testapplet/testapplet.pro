TEMPLATE = lib
CONFIG += plugin \
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

MOC_DIR = .moc
OBJECTS_DIR = .objects

HEADERS = \
    ../debug.h \
    testapplet.h \
    testwidget.h \
    testbrief.h 

SOURCES = \
    ../debug.cpp \
    testapplet.cpp \
    testwidget.cpp \
    testbrief.cpp 

DESTDIR = lib
desktop.files += *.desktop
desktop.path = $$(DEBIAN_DESTDIR)/usr/lib/mcontrolpanel
target.path += $$(DEBIAN_DESTDIR)$$[QT_INSTALL_LIBS]/mcontrolpanel/applets

message("The plugin will be installed to: " $$target.path)
INSTALLS += target \
            css \
            desktop
