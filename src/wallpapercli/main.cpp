/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Karoliina T. Salminen <karoliina.t.salminen@nokia.com>
**
** This file is part of duicontrolpanel.
**
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/
#include <stdio.h>
#include <getopt.h>
#include <libgen.h>

#include <DuiControlPanelIf>
#include <MDialog>
#include <MApplication>
#include <MApplicationWindow>
#include <MApplicationPage>

#define DEBUG
#include "../debug.h"


static struct option
long_options[] =
{
    {"help",            no_argument, 0, 'h'},
    {"start-applet",    no_argument, 0, 's'},
    {0, 0, 0, 0}
};

static const char *short_options = "hs";

static char        *programName = NULL;
DuiControlPanelIf  *m_DcpIf = NULL;
bool                m_OptionStartApplet = false;

void
printHelpAndExit ()
{
    fprintf (stderr, 
"Usage: %s [OPTION...]\n\n"
"-h, --help          Print help message and exit.\n"
"-s, --start-applet  Start the wallpaper applet."
"\n\n", programName
);
    exit (EXIT_SUCCESS);
}

void
startControlPanel ()
{
    SYS_DEBUG ("");
    system ("duicontrolpanel.launch -software >/dev/null &");
    sleep (1);
}

void
pageToWallpaperApplet ()
{
    SYS_DEBUG ("");
    Q_ASSERT (m_DcpIf);
    m_DcpIf->appletPage("Wallpaper");
}

int 
main (
        int argc, 
		char *argv[])
{
    int c;
    int digit_optind = 0;

    programName = basename (argv[0]);

    while (1) {
        int this_option_optind = optind ? optind : 1;
        int option_index = 0;
        
        c = getopt_long (argc, argv, 
                short_options, long_options, &option_index);

        if (c == -1)
            break;

            switch (c) {
                case 's':
                    m_OptionStartApplet = true;
                    break;
                case 'h':
                    printHelpAndExit ();
                    break;
            }
    }

    m_DcpIf = new DuiControlPanelIf();

    // check the interface is valid
    if (!m_DcpIf->isValid()) {
        SYS_WARNING ("Err: Service unavailable");
        exit (EXIT_FAILURE);
    }

    if (m_OptionStartApplet) {
        startControlPanel ();
        pageToWallpaperApplet ();
    }

    return 0;
}

