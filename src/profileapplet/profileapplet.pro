TEMPLATE = lib
CONFIG += plugin \
    gui \
    m \
    silent \
    debug
LIBS += -lmcontrolpanel \
    -lprofile-qt
INCLUDEPATH += /usr/include/qt4/m \
    /usr/include/mcontrolpanel \
    /usr/include/profile-qt
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
    profileapplet.h \
    profilewidget.h \
    profilebrief.h \
    profilecontainer.h \
    profilebuttons.h \
    profiledatainterface.h
SOURCES = \
    ../debug.cpp \
    profileapplet.cpp \
    profilewidget.cpp \
    profilebrief.cpp \
    profilecontainer.cpp \
    profilebuttons.cpp \
    profiledatainterface.cpp
css.files = profileapplet.css
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
