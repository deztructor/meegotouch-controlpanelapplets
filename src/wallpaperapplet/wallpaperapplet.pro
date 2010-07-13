include(../coverage.pri)

TEMPLATE = lib
MOC_DIR = .moc
OBJECTS_DIR = .objects

CONFIG += plugin \
          gui \
          meegotouch \
          duicontrolpanel \
	  silent

LIBS += \
    -lqttracker \
    -lthumbnailer

INCLUDEPATH += \
    /usr/include/QtTracker \
    /usr/include/thumbnailer 


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

backup.files = wallpaper.conf
backup.path  = /usr/share/backup-framework/applications/

backupscripts.files = wallpaper-backup wallpaper-restore
backupscripts.path  = $$(DEBIAN_DESTDIR)/usr/share/wallpaper/

desktop.files += *.desktop
desktop.path = $$(DEBIAN_DESTDIR)/usr/lib/duicontrolpanel

css.files = wallpaper.css
css.path = \
  $$(DEBIAN_DESTDIR)/usr/share/themes/base/meegotouch/duicontrolpanel/style

images.files = images/*.png
images.path  = $$css.path/images

message("target path         :" $$target.path)
message("rfs path            :" $$rfs.path)
message("desktop path        :" $$desktop.path)
message("css path:           :" $$css.path)
message("images path         :" $$images.path)
message("backup conf path    :" $$backup.path)
message("backup scripts path :" $$backups.path)

INSTALLS += \
    target \
    images \
    css \
    rfs \
    desktop \
    backupscripts \
    backup

