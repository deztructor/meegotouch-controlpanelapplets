TEMPLATE = lib
CONFIG += plugin \
          gui \
          meegotouch \
          duicontrolpanel \
          silent

LIBS += \
    -lqttracker \
    -lduithumbnailer

INCLUDEPATH += \
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
    wallpapercurrentdescriptor.h \
    wallpapermodel.h \
    wallpaperlist.h \
    wallpaperbusinesslogic.h \
    wallpaperapplet.h \
    wallpaperwidget.h \
    wallpaperinfoheader.h \
    wallpapereditorwidget.h \
    wallpaperitrans.h \
    wallpaperbrief.h 

SOURCES = \
    ../debug.cpp \
    wallpaperdescriptor.cpp \
    wallpapercurrentdescriptor.cpp \
    wallpaperlist.cpp \
    wallpapermodel.cpp \
    wallpaperbusinesslogic.cpp \
    wallpaperapplet.cpp \
    wallpaperwidget.cpp \
    wallpaperinfoheader.cpp \
    wallpapereditorwidget.cpp \
    wallpaperitrans.cpp \
    wallpaperbrief.cpp 

DESTDIR = lib
target.path += $$(DEBIAN_DESTDIR)$$[QT_INSTALL_LIBS]/duicontrolpanel/applets

rfs.files = wallpaper-rfs.sh
rfs.path  = $$system(pkg-config --variable osso_rfs_scripts_dir clean-device)

desktop.files += *.desktop
desktop.path = $$(DEBIAN_DESTDIR)/usr/lib/duicontrolpanel

css.files = wallpaper.css
css.path = \
  $$(DEBIAN_DESTDIR)/usr/share/themes/base/meegotouch/duicontrolpanel/style

images.files = images/*.png
images.path  = $$css.path/images

message("target path   :" $$target.path)
message("rfs path      :" $$rfs.path)
message("desktop path  :" $$desktop.path)
message("css path:     :" $$css.path)
message("images path   :" $$images.path)

INSTALLS += \
    target \
    images \
    css \
    rfs \
    desktop
