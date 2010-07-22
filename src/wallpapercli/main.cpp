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
#include "wallpapergconf.h"

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
    {"edit",               no_argument, 0, 'e'},
    {"portrait",           required_argument, 0, 'p'},
    {"landscape",          required_argument, 0, 'l'},

    {0, 0, 0, 0}
};

static const char *short_options = "hsp:l:Se";

WallpaperCLI::WallpaperCLI () :
    m_ProgramName (0),
    m_OptionHelp (false),
    m_OptionVerbose (false),
    m_PrintGconf (false),
    m_SetGconf (false),
    m_EditImages (false),
    m_OptionStartApplet (false),
    m_DcpIf (new DuiControlPanelIf),
    m_ExitCode (EXIT_SUCCESS)
{
    SYS_DEBUG ("landscape-key: %s", WALLPAPER_LANDSCAPE_KEY);
    SYS_DEBUG ("portrait-key : %s", WALLPAPER_PORTRAIT_KEY);
    m_LandscapeGConfItem = new MGConfItem (WALLPAPER_LANDSCAPE_KEY);
    m_PortraitGConfItem = new MGConfItem (WALLPAPER_PORTRAIT_KEY);
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

                case 'e':
                    m_EditImages = true;
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
                    m_OptionPortraitFilename = argv[optind - 1];
                    break;

                case 'l':
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
    bool retval = true;

    /*
     * Executing --help: printing the help message and exit.
     */
    if (m_OptionHelp) {
        printHelp ();
        goto finalize;
    } 
    
    /*
     * Executing --set-gconf: Simply changing the file names in the GConf
     * database. 
     */
    if (m_SetGconf) {
        SYS_DEBUG ("Setting GConf keys.");
        SYS_DEBUG ("land: %s", SYS_STR(m_OptionLandscapeFilename));
        SYS_DEBUG ("port: %s", SYS_STR(m_OptionPortraitFilename));
        m_LandscapeGConfItem->set (m_OptionLandscapeFilename);
        m_PortraitGConfItem->set (m_OptionPortraitFilename);
        
        goto finalize;
    }
    
    /*
     * Printing the values stored in the GConf database.
     */
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

    /*
     * Executing --edit: Opening the images in the wallpaper applet for 
     * editing.
     */
    if (m_EditImages) {
        retval = performEditImages ();
        goto finalize;
    }

    /*
     * Executing --start-applet: Start up the applet in normal mode.
     */
    if (m_OptionStartApplet) {
        retval = pageToWallpaperApplet ();
        goto finalize;
    }

    printHelp ();

finalize:
    return true;
}

bool
WallpaperCLI::performEditImages ()
{
    MGConfItem lEditedImage (WALLPAPER_EDITED_LANDSCAPE_KEY);
    MGConfItem pEditedImage (WALLPAPER_EDITED_PORTRAIT_KEY);
    MGConfItem requestCode  (WALLPAPER_APPLET_REQUEST_CODE);
    bool retval = true;

    SYS_DEBUG ("+++ Resetting GConf...");
    requestCode.set (-1);

    SYS_DEBUG ("+++ Pushing request into GConf...");
    lEditedImage.set (m_OptionLandscapeFilename);
    pEditedImage.set (m_OptionPortraitFilename);
    requestCode.set (WallpaperRequestEdit);

    SYS_DEBUG ("+++ Starting up the controlpanel");
    if (!pageToWallpaperApplet()) {
        retval = false;
        goto finalize;
    }

finalize:
    return retval;
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
"--edit                     Edit the images in the wallpaper applet.\n"
"\n"
"-p, --portrait <filename>  Portrait image filename.\n"
"-l, --landscape <filename> Portrait image filename.\n"
"\n"
"Examples:\n\n"
"  Starting to edit\n"
"    %s --portrait image1.jpg --landscape image2.jpg --edit\n\n"
"  Set wallpaper without editing\n"
"    %s --portrait image1.jpg --landscape image2.jpg --set-gconf\n\n"
"  Show settings\n"
"    %s --verbose --print-gconf\n\n"
"", m_ProgramName, m_ProgramName, m_ProgramName, m_ProgramName
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
    system ("duicontrolpanel.launch &");
    sleep (1);
}

bool
WallpaperCLI::pageToWallpaperApplet ()
{
    SYS_DEBUG ("");
    Q_ASSERT (m_DcpIf);
   
    /*
     * FIXME: It is not clear why do we need to to start up the controlpanel
     * manually here...
     */
    startControlPanel ();

    if (!m_DcpIf->isValid()) {
        SYS_WARNING ("ERROR: Service unavailable");
        m_ErrorString = "Service unavailable";
        m_ExitCode = EXIT_FAILURE;
        return false;
    }
        
    return m_DcpIf->appletPage("Wallpaper");
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

