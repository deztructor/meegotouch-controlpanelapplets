#! /bin/sh
# USB applet
gconftool-2 --recursive-unset /Meego/System/UsbMode
# Display settings applet
gconftool-2 --recursive-unset /system/osso/dsm/display
# Battery settings applet
gconftool-2 --recursive-unset /system/osso/dsm/energymanagement
# Wallpaper settings applet
gconftool-2 --recursive-unset /desktop/meego/background
rm -rf $HOME/.wallpapers
# Soundsettings applet
gconftool-2 --recursive-unset /meegotouch/input_feedback
# Theme applet
gconftool-2 --recursive-unset /meegotouch/theme

