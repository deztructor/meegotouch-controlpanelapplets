#!/bin/sh
#
# This script is temporary, will be removed once the new wallpaper
# applet is used by default, for now it is under development.
#
INSTALL_FILE=debian/duicontrolpanel-wallpaperapplet.install

LIBNAME=libwallpaperapplet
DESKTOPNAME=wallpaper
echo '##########################################################'
if [ "$1x" == "improvedx" ]; then
    LIBNAME=libwallpaperapplet2
    DESKTOPNAME=wallpaper2
    echo '##### Building package with the NEW wallpaper applet #####'
else
    echo '##### Building package with the old wallpaper applet #####'
fi
echo '##########################################################'

cat - > ${INSTALL_FILE} <<CONTENTS
usr/share/themes/base/meegotouch/libwallpaperapplet/style/${LIBNAME}.css
usr/lib/duicontrolpanel/${DESKTOPNAME}.desktop
usr/lib/duicontrolpanel/applets/${LIBNAME}.so
usr/share/backup-framework/applications/wallpaper.conf
CONTENTS

