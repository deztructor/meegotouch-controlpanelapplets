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
    themedescriptor.h \
    themebusinesslogic.h \
    themeapplet.h \
    themewidget.h \
    themebrief.h \
    themecontainer.h \
    themedialog.h

SOURCES = \
    ../debug.cpp \
    themedescriptor.cpp \
    themebusinesslogic.cpp \
    themeapplet.cpp \
    themewidget.cpp \
    themebrief.cpp \
    themecontainer.cpp \
    themedialog.cpp

css.files = themeapplet.css
DESTDIR = lib
desktop.files += *.desktop
desktop.path = $$(DEBIAN_DESTDIR)/usr/lib/mcontrolpanel
target.path += $$(DEBIAN_DESTDIR)$$[QT_INSTALL_LIBS]/mcontrolpanel/applets
css.path += $$(DEBIAN_DESTDIR)/usr/share/themes/base/m/mcontrolpanel/style

message("The plugin will be installed to: " $$target.path)
message("CSS path will be: " $$css.path)
INSTALLS += target \
            css \
            desktop
