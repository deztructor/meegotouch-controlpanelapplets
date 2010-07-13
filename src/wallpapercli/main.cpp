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
#include <MGConfItem>

#include "main.h"

#define DEBUG
#include "../debug.h"

static struct option
long_options[] =
{
    {"help",               no_argument, 0, 'h'},
    {"verbose",            no_argument, 0, 'V'},
    {"start-applet",       no_argument, 0, 's'},
    {"print-gconf",        no_argument, 0, 'P'},
    {"set-gconf",          no_argument, 0, 'S'},
    {"portrait",           required_argument, 0, 'p'},
    {"landscape",          required_argument, 0, 'l'},

    {0, 0, 0, 0}
};

static const char *short_options = "hsp:l:S";

static const QString PortraitKey = 
    "/desktop/meego/background/portrait/picture_filename";
static const QString LandscapeKey = 
    "/desktop/meego/background/landscape/picture_filename";


WallpaperCLI::WallpaperCLI () :
    m_ProgramName (0),
    m_OptionHelp (false),
    m_OptionVerbose (false),
    m_PrintGconf (false),
    m_OptionStartApplet (false),
    m_DcpIf (new DuiControlPanelIf),
    m_ExitCode (EXIT_SUCCESS)
{
    m_LandscapeGConfItem = new MGConfItem (LandscapeKey);
    m_PortraitGConfItem = new MGConfItem (PortraitKey);
}

WallpaperCLI::~WallpaperCLI ()
{
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

        //SYS_DEBUG ("*** c            = %c", c);
        //SYS_DEBUG ("*** option_index = %d", option_index);
        //SYS_DEBUG ("*** optind       = %d", optind);

        if (c == -1)
            break;

            switch (c) {
                case 's':
                    m_OptionStartApplet = true;
                    break;
                
                case 'S':
                    m_SetGconf = true;
                    break;

                case 'h':
                    m_OptionHelp = true;
                    break;

                case 'V':
                    m_OptionVerbose = true;
                    break;

                case 'P':
                    m_PrintGconf = true;
                    break;
                
                case 'L':
                    m_SetGconf = true;
                    break;
                
                case 'p':
                    SYS_DEBUG ("p = %s", argv[optind - 1]);
                    m_OptionPortraitFilename = argv[optind - 1];
                    break;

                case 'l':
                    SYS_DEBUG ("l = %s", argv[optind - 1]);
                    m_OptionLandscapeFilename = argv[optind - 1];
                    break;

                default:
                    SYS_WARNING ("Unhandled option: %d", c); 
            }
    }

    return true;
}

bool
WallpaperCLI::executeCommandLineArguments ()
{
    if (m_OptionHelp) {
        printHelp ();
        goto finalize;
    } 
    
    if (m_SetGconf) {
        m_LandscapeGConfItem->set (m_OptionLandscapeFilename);
        m_PortraitGConfItem->set (m_OptionPortraitFilename);
        
        goto finalize;
    }
    
    if (m_PrintGconf) {
        QString landscape = m_LandscapeGConfItem->value().toString();
        QString portrait = m_PortraitGConfItem->value().toString();

        if (m_OptionVerbose) {
            printf ("Landscape : '%s'\n", landscape.toLatin1().constData());
            printf ("Portrait  : '%s'\n", portrait.toLatin1().constData());
        } else {
            printf ("'%s'\n", landscape.toLatin1().constData());
            printf ("'%s'\n", portrait.toLatin1().constData());
        }

        goto finalize;
    }

    if (m_OptionStartApplet) {
        if (!m_DcpIf->isValid()) {
            m_ErrorString = "Service unavailable";
            m_ExitCode = EXIT_FAILURE;
            return false;
        }

        startControlPanel ();
        pageToWallpaperApplet ();
    }

finalize:
    return true;
}


void
WallpaperCLI::printHelp ()
{
    fprintf (stderr, 
"Usage: %s [OPTION...]\n\n"
"-h, --help                 Print help message and exit.\n"
"--verbose                  Verbose output.\n"
"\n"
"-s, --start-applet         Start the wallpaper applet.\n"
"--print-gconf              Print the settings from the GConf database.\n"
"--set-gconf                Save the settings from the GConf database.\n"
"\n"
"-p, --portrait <filename>  Portrait image filename.\n"
"-l, --landscape <filename> Portrait image filename.\n"
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

