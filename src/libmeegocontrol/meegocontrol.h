/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of meegotouch-controlpanelapplets.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/
#ifndef MEEGOCONTROL_H
#define MEEGOCONTORL_H

/*! \mainpage libMeegoControl
 *
 * \section intro_sec Introduction
 * The libMeegoControl library is a very simple library created using the Meego
 * Control Panel Applets as base source. The primary repository for the Meego
 * Control Panel Applets can be found at the following URL.
\verbatim
http://meego.gitorious.org/meegotouch/meegotouch-controlpanelapplets
\endverbatim
 *
 * The code that is re-used for this library is designed to operate in the Meego
 * Controlpanel as a GUI applet, it is not intended to be used as a standalone
 * library. Taking part of the applet code and using it outside its normal
 * operational environment is highly experimental and while it is stable, it
 * might be not as efficient as it could be.
 *
 * Currently the libMeegoControl library holds only a few classes from the
 * controlpanel applets to support the setting of the wallpaper, the background
 * image for the meego desktop and the meego lockscreen. More functionality
 * might be added later.
 *
 * If you have any problems using the library please file a bug at
 * https://projects.maemo.org/bugzilla/ in the H-Setting->wallpaperapplet
 * component.
 *
 * \section using Using the library
 * To use the library first it has to be added to the configuration of the
 * application project file:
\code
CONFIG += \
    meegocontrol \
    silent
\endcode
 *
 * To set the wallpaper one must create a WallpaperDescriptor that holds the
 * information about a set of images (usually one image for landscape and one
 * more image for portrait orientation), two objects of type WallpaperITrans
 * that describes the image transformations for the two orientation and finally
 * a WallpaperBusinessLogic object that does the job.
 *
 * Here is a small example that sets the wallpaper using two image files. If the
 * same image file is intended for both orientations, the same filename can be
 * passed twice.
\code
#include <meegocontrol.h>

int
main (int argc, char** argv)
{
        QApplication           app (argc, argv);
        WallpaperBusinessLogic *logic;
        WallpaperDescriptor    *desc;
        WallpaperITrans         landscapeTrans;
        WallpaperITrans         portraitTrans;
        QString                 file1;
        QString                 file2;

    // Checking the command line arguments.
    if (!argv[1] || !argv[2]) {
        fprintf (stderr, "Usage: \n  %s <imagefile1> <imagefile2>\n", argv[0]);
        exit (1);
    }

    file1 = argv[1];
    file2 = argv[2];

    // Initializing.
    logic = new WallpaperBusinessLogic;
    desc = new WallpaperDescriptor;

    // Setting the wallpaper here.
    desc->setFilename (file1, WallpaperDescriptor::Landscape);
    desc->setFilename (file2, WallpaperDescriptor::Portrait);
    logic->setBackground (&landscapeTrans, &portraitTrans, desc);

    // Releasing resources.
    delete desc;
    delete logic;
    return 0;
}
\endcode
 *
 * Once the wallpaper set the files are saved into the ~/.wallpapers directory
 * of the user's home and the actual settings are changed in the GConf database.
 */
#include <WallpaperBusinessLogic>
#include <WallpaperDescriptor>
#include <WallpaperCurrentDescriptor>
#include <WallpaperITrans>

#endif
