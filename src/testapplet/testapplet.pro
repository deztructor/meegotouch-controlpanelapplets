TEMPLATE = lib
CONFIG += plugin \
    gui \
    dui \
    silent \
    debug

LIBS += -lduicontrolpanel 

INCLUDEPATH += /usr/include/qt4/dui \
    /usr/include/duicontrolpanel 

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
desktop.path = $$(DEBIAN_DESTDIR)/usr/lib/duicontrolpanel
target.path += $$(DEBIAN_DESTDIR)$$[QT_INSTALL_LIBS]/duicontrolpanel/applets

message("The plugin will be installed to: " $$target.path)
INSTALLS += target \
            css \
            desktop
