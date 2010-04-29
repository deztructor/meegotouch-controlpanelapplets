TEMPLATE = lib
CONFIG += plugin \
    gui \
    meegotouch \
    silent \
    debug

LIBS += \
    -lduicontrolpanel \
    -lqttracker \
    -lduithumbnailer

INCLUDEPATH += \
    /usr/include/duicontrolpanel \
    /usr/include/QtTracker \
    /usr/include/duithumbnailer 

contains(cov, true) { 
    message("Coverage options enabled")
    QMAKE_CXXFLAGS += --coverage
    QMAKE_LFLAGS += --coverage
}

MOC_DIR = .moc
OBJECTS_DIR = .objects

HEADERS = \
    ../debug.h \
    wallpaperdescriptor.h \
    wallpapermodel.h \
    wallpaperlist.h \
    wallpaperbusinesslogic.h \
    wallpaperapplet.h \
    wallpaperwidget.h \
    wallpapereditorwidget.h \
    wallpaperitrans.h \
    wallpaperbrief.h 

SOURCES = \
    ../debug.cpp \
    wallpaperdescriptor.cpp \
    wallpaperlist.cpp \
    wallpapermodel.cpp \
    wallpaperbusinesslogic.cpp \
    wallpaperapplet.cpp \
    wallpaperwidget.cpp \
    wallpapereditorwidget.cpp \
    wallpaperitrans.cpp \
    wallpaperbrief.cpp 

css.files = wallpaper.css
DESTDIR = lib
rfs.files += wallpaper-rfs.sh
rfs.path += /etc/osso-rfs-scripts
desktop.files += *.desktop
desktop.path = $$(DEBIAN_DESTDIR)/usr/lib/duicontrolpanel
target.path += $$(DEBIAN_DESTDIR)$$[QT_INSTALL_LIBS]/duicontrolpanel/applets
css.path += $$(DEBIAN_DESTDIR)/usr/share/themes/base/meegotouch/duicontrolpanel/style

message("The plugin will be installed to: " $$target.path)
message("CSS path will be: " $$css.path)
INSTALLS += target \
            css \
	    rfs \
            desktop
