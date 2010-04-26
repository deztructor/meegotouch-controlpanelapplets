#! /bin/sh

#
# The 'reset factory settings' script for the theme applet.
#

# FIXME: Nothing seems to work...
#gconftool-2 --type string --set /meegotouch/theme/name ""
gconftool-2 --recursive-unset /meegotouch/theme/name

