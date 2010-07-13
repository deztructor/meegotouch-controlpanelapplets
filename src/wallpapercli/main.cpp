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

#include "main.h"

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


WallpaperCLI::WallpaperCLI () :
    m_ProgramName (0),
    m_OptionHelp (false),
    m_OptionStartApplet (false),
    m_DcpIf (new DuiControlPanelIf),
    m_ExitCode (EXIT_SUCCESS)
{
}

WallpaperCLI::~WallpaperCLI ()
{
    if (m_DcpIf)
        delete m_DcpIf;
}


bool 
WallpaperCLI::parseCommandLineArguments (
        int argc, char *argv[])
{
    int c;
    //int digit_optind = 0;

    m_ProgramName = basename (argv[0]);

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
                    m_OptionHelp = true;
                    break;

                default:
                    SYS_WARNING ("argv[%d] = %s", 
                            option_index, argv[option_index]);
            }
    }

    return true;
}

bool
WallpaperCLI::executeCommandLineArguments ()
{
    if (m_OptionHelp) {
        printHelp ();
    } else if (m_OptionStartApplet) {
        if (!m_DcpIf->isValid()) {
            m_ErrorString = "Service unavailable";
            m_ExitCode = EXIT_FAILURE;
            return false;
        }

        startControlPanel ();
        pageToWallpaperApplet ();
    }

    return true;
}


void
WallpaperCLI::printHelp ()
{
    fprintf (stderr, 
"Usage: %s [OPTION...]\n\n"
"-h, --help          Print help message and exit.\n"
"-s, --start-applet  Start the wallpaper applet."
"\n\n", m_ProgramName
);
}

void
WallpaperCLI::printError ()
{
    SYS_WARNING ("Error: %s", SYS_STR(m_ErrorString));
}

void
WallpaperCLI::startControlPanel ()
{
    SYS_DEBUG ("");
    system ("duicontrolpanel.launch -software >/dev/null &");
    sleep (1);
}

void
WallpaperCLI::pageToWallpaperApplet ()
{
    SYS_DEBUG ("");
    Q_ASSERT (m_DcpIf);
    m_DcpIf->appletPage("Wallpaper");
}

int 
main (
        int   argc, 
		char *argv[])
{
    WallpaperCLI cli;
    bool         success;

    success = cli.parseCommandLineArguments (argc, argv);
    if (!success) {
        cli.printError ();
        goto finalize;
    }

    success = cli.executeCommandLineArguments ();
    if (!success) {
        cli.printError ();
    }

finalize:
    return cli.exitCode ();
}

