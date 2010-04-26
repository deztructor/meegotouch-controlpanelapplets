#! /bin/sh

#
# The 'reset factory settings' script for the theme applet.
#

gconftool-2 --recursive-unset /meegotouch/theme/name

