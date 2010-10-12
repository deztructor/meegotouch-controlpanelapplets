#! /bin/sh
# Un-set the keys...
gconftool-2 --recursive-unset /desktop/meego/background
rm -rf $HOME/.wallpapers

