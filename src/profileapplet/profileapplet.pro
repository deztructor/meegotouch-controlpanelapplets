include(../../localconfig.pri)
include(../coverage.pri)

TEMPLATE = lib
CONFIG += plugin \
          gui \
          meegotouch \
          duicontrolpanel \
          silent

contains(DEFINES, HAVE_PROFILE_QT) {
    LIBS += -lprofile-qt
    INCLUDEPATH += /usr/include/profile-qt
}

QT += dbus
MOC_DIR = .moc
OBJECTS_DIR = .objects
HEADERS = \
    ../debug.h \
    profileapplet.h \
    profilewidget.h \
    profilecontainer.h \
    profiledatainterface.h
SOURCES = \
    ../debug.cpp \
    profileapplet.cpp \
    profilewidget.cpp \
    profilecontainer.cpp \
    profiledatainterface.cpp

css.files = profileapplet.css
DESTDIR = lib
desktop.files += *.desktop
desktop.path = $$(DEBIAN_DESTDIR)/usr/lib/duicontrolpanel
target.path += $$(DEBIAN_DESTDIR)$$[QT_INSTALL_LIBS]/duicontrolpanel/applets
css.path += $$(DEBIAN_DESTDIR)/usr/share/themes/base/meegotouch/duicontrolpanel/style

message("The plugin will be installed to: " $$target.path)
message("CSS path will be: " $$css.path)
INSTALLS += target \
            css \
            desktop
