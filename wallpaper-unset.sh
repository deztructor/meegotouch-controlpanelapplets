#!/bin/bash

gconftool-2 --recursive-unset /desktop/meego/background
sleep 3
gconftool-2 --recursive-list /desktop/meego/background
