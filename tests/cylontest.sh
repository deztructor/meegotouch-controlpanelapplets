#! /bin/sh
# -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- 
# vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" 

#
# Configuration variables.
#
WALLPAPER_FILE=/usr/share/themes/base/meegotouch/images/meegotouch-wallpaper-portrait.jpg


#
# Other global variables.
#
TERM_YELLOW="\033[1;31m" 
TERM_RED="\033[1;33m" 
TERM_GREEN="\033[1;32m"
TERM_NORMAL="\033[0;39m"
TERM_BOLD="\033[1m"



TEMP=`getopt -n cylontest.sh -o \
h \
--long \
verbose,\
perform:,\
help -- "$@"`

if [ $? != 0 ] ; then echo "Terminating..." >&2 ; exit 1 ; fi

# Note the quotes around `$TEMP': they are essential!
eval set -- "$TEMP"

while true ; do
    case "$1" in

	--verbose) 
	    VERBOSE="yes"
	    shift 
	    ;;

	--perform)
	    PERFORM="$2"
	    shift 2 
	    ;;

	-h|--help)
	    echo "cylontest.sh - Utility script for the Cylon group."
	    echo "usage: $(basename $0) [options]"
	    echo "options:"
	    echo "  --verbose       Print more information"
	    echo "  -h, --help      Display this help and exit"
	    echo "  --perform NAME  Perform the named test"
        echo ""
        echo "    Valid names are:"
        echo "      o wallpaperedit"
        echo "      o themepackage"
	    exit 0
	    ;;

	--) shift ; break 
	    ;;

	*) echo "Internal error!  no case for option \"$1\""
	    exit 1 
	    ;;
    esac
done

debug_message () {
    if [ "$VERBOSE" != "yes" ]; then
        return 0
    fi

    echo $*
}

print () {
    echo -e $*
}

initialize () {
    for file in /tmp/session_bus_address.*; do
        debug_message "Sourcing $file..."
        source $file
    done

    export DISPLAY=:0
}

performtest () {
    case "$1" in
        themepackage) 
            print "             ${TERM_BOLD}Performing theme dbus test.${TERM_NORMAL}"
            cat <<EOF

  This test will start up the Theme Applet, then will  use its DBus API to 
  remove and add themes. The theme packages will use this DBus API to notify the
  applet about the installation and removal of the themes.

  The test adds back all the themes it removes, so when it is performed the same
  list should be shown that was originally shown. 
EOF
            debug_message "(1)  Starting up the controlpanel"
            dbus-send \
                --print-reply \
                --session \
                --dest=com.nokia.DuiControlPanel \
                    / com.nokia.DuiControlPanelIf.appletPage string:Theme
            sleep 2

            debug_message "(2)  Waiting"
            sleep 2

            debug_message "(3)  Removing blanco theme"
            dbus-send --session --type=signal \
                / com.nokia.theme.removed string:blanco
            sleep 2

            debug_message "(4)  Removing plankton theme"
            dbus-send --session --type=signal \
                / com.nokia.theme.removed string:plankton
            sleep 2

            debug_message "(5)  Adding plankton theme"
            dbus-send --session --type=signal \
                / com.nokia.theme.added string:plankton

            debug_message "(6)  Adding blanco theme"
            dbus-send --session --type=signal \
                / com.nokia.theme.added string:blanco
            sleep 2
            ;;

        wallpaperedit) 
            print "             ${TERM_BOLD}Performing wallpaperedit test.${TERM_NORMAL}"
            cat <<EOF

  This test first starts up the controlpanel through a dbus call to show the
  Wallpaper Applet. When the applet is shown an other dbus call will be sent to
  page to the Wallpaper Editor widget, the second page of the applet. Here an
  image will be opened so that the user can edit and accept the image as
  wallpaper for the desktop and the Lock Screen UI.
  
  After the test is performed the Wallpaper Applet should be on screen with the
  image editor page.

EOF
            debug_message "(1)  Starting up the controlpanel"
            dbus-send \
                --print-reply \
                --session \
                --dest=com.nokia.DuiControlPanel \
                    / com.nokia.DuiControlPanelIf.appletPage string:Wallpaper

            debug_message "(2)  Waiting"
            sleep 2

            debug_message "(3)  Starting up the editor through DBus"
            dbus-send \
                --session \
                --type=signal \
                / com.nokia.wallpaper.edit \
                string:${WALLPAPER_FILE} \
                string:${WALLPAPER_FILE} 

            debug_message "(2)  Waiting"
            sleep 8
	        ;;

        *) 
            echo "No such test define: $1"
    	    exit 1 
	        ;;
    esac
}

#
# This will do all the initialization.
#
initialize

if [ ! -z "$PERFORM" ]; then
    performtest $PERFORM
    exit 0
fi

exit 0
