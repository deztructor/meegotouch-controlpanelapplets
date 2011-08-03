/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "ut_wallpapermodel.h"
#include "wallpaperutils_stub.h"

#include "wallpapermodel.h"
#include "wallpaperdescriptor.h"
#include "wallpaperbusinesslogic.h"
#include "wallpaperconfiguration.h"
#include "wallpaperutils.h"

#include <MGConfItem>
#include <MApplication>

#define DEBUG
#include "../../src/debug.h"

static const QString PortraitKey =
    "/desktop/meego/background/portrait/picture_filename";
static const QString LandscapeKey =
    "/desktop/meego/background/landscape/picture_filename";



/******************************************************************************
 * WallpaperModelSignals implementation. 
 */
void
WallpaperModelSignals::currentChanged (
        const QModelIndex &current)
{
    SYS_DEBUG ("");
}



/******************************************************************************
 * Ut_WallpaperModel implementation. 
 */
void 
Ut_WallpaperModel::init()
{
#if 0
    MGConfItem gconf_portrait (PortraitKey);
    MGConfItem gconf_landscape (LandscapeKey);
    /*
     * In case if gconf values are not valid, we
     * don't have to fail... lets set some fake values
     */
    if (gconf_portrait.value ().toString ().isEmpty ())
        gconf_portrait.set ("meegotouch-wallpaper-portrait");
    if (gconf_landscape.value ().toString ().isEmpty ())
        gconf_landscape.set ("meegotouch-wallpaper-landscape");
#endif
}

void 
Ut_WallpaperModel::cleanup()
{
}


static int argc = 1;
static char *app_name = (char*) "./ut_wallpapermodel";

void 
Ut_WallpaperModel::initTestCase()
{
    bool signalSuccess;

    stubAddFileToFilesystem ("/testFile1.png", 100);
    stubAddFileToFilesystem ("/testFile2.png", 100);

    m_App = new MApplication (argc, &app_name);
    m_BusinessLogic = new WallpaperBusinessLogic;
    m_Model = new WallpaperModel (m_BusinessLogic);

    signalSuccess = connect (
            m_Model, SIGNAL(currentChanged(const QModelIndex &)),
            &m_Signals, SLOT(currentChanged(const QModelIndex &)));
    QVERIFY (signalSuccess);

    QVERIFY (m_Model->m_ImagesDir == 
            Wallpaper::constructPath (Wallpaper::ImagesDir));
    QVERIFY (m_Model->m_SysImagesDir == 
            Wallpaper::constructPath (Wallpaper::SysImagesDir));

    #if 0
    SYS_DEBUG ("*** rowCount() = %d", m_Model->rowCount());
    for (int n = 0; n < m_Model->m_FilePathList.size(); ++n) {
        SYS_DEBUG ("[%03d] %s", n, SYS_STR(m_Model->m_FilePathList[n]));
    }
    #endif
    
    /*
     * Re-calling the loadFromDirectory() is delayed, but we force calling it
     * now.
     */
    m_Model->loadFromDirectory ();
    SYS_DEBUG ("*** rowCount() = %d", m_Model->rowCount());
    for (int n = 0; n < m_Model->m_FilePathList.size(); ++n) {
        SYS_DEBUG ("[%03d] %s", n, SYS_STR(m_Model->m_FilePathList[n]));
    }

    /*
     * We must have one current image and the two simulated files in the model.
     */
    QVERIFY(m_Model->rowCount() == 3);
}

void 
Ut_WallpaperModel::cleanupTestCase()
{
    delete m_BusinessLogic;
    delete m_Model;
    m_App->deleteLater ();
}


QTEST_APPLESS_MAIN(Ut_WallpaperModel)


