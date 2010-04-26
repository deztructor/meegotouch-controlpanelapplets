#! /bin/sh

#
# The 'reset factory settings' script for the wallpaper applet.
#

gconftool-2 --recursive-unset /desktop/standard/background
