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

#include "ft_wallpaperdescriptor.h"

#include "wallpaperdescriptor.h"
#include "wallpapercurrentdescriptor.h"
#include "wallpaperitrans.h"

#include <QPixmap>
#include <QUrl>
#include <QFile>
#include <QString>

#include <MApplication>
#include <MGConfItem>

#define DEBUG
#include "../../src/debug.h"

/*
 * These should always be on the device. At least I hope so!
 */
static const QString defaultLandscapeImageFile = 
"/usr/share/themes/base/meegotouch/duihome/images/HomeWallpaperLandscape.png";

static const QString defaultPortraitImageFile = 
"/usr/share/themes/base/meegotouch/duihome/images/HomeWallpaperPortrait.png";

static const QString defaultLandscapeMimeType = "image/png";

/******************************************************************************
 * SignalSink implementation.
 */
SignalSink::SignalSink ()
{
    reset ();
}

void 
SignalSink::reset ()
{
    m_ThumbnailLoaded = false;
}

void
SignalSink::thumbnailLoaded (
        WallpaperDescriptor *desc)
{
    Q_UNUSED (desc);
    SYS_DEBUG ("Thumbnail loaded for file %s", SYS_STR(desc->filename()));

    m_ThumbnailLoaded = true;
}


/******************************************************************************
 * Ft_WallpaperDescriptor implementation. 
 */
void 
Ft_WallpaperDescriptor::init()
{
}

void 
Ft_WallpaperDescriptor::cleanup()
{
}


static int argc = 1;
static char* app_name = (char*) "./Ft_WallpaperDescriptor";

void 
Ft_WallpaperDescriptor::initTestCase()
{
    m_App = new MApplication (argc, &app_name);
    m_Desc = 0;
}

void 
Ft_WallpaperDescriptor::cleanupTestCase()
{
    if (m_Desc)
        delete m_Desc;

    m_App->deleteLater ();
}

/*!
 * Will call the cache() method to load an image and then will call the
 * unCache() method
 */
void 
Ft_WallpaperDescriptor::testCacheUncache ()
{
    QFile file (defaultLandscapeImageFile);
    if (!file.exists()) {
        SYS_WARNING (
"File %s does not exists. This test is aborted and a success will be reported."
"I have no idea what is happening with this file!!", 
        SYS_STR(defaultLandscapeImageFile));
        return;
    }

    createDescriptor ();

    m_Desc->setFilename (defaultLandscapeImageFile);
    m_Desc->cache ();

    QVERIFY (m_Desc->m_Images[WallpaperDescriptor::Landscape].m_Cached);
    QVERIFY (m_Desc->m_Images[WallpaperDescriptor::Landscape].m_Pixmap.width() == 864);
    QVERIFY (m_Desc->m_Images[WallpaperDescriptor::Landscape].m_Pixmap.height() == 480);

    m_Desc->unCache ();
    QVERIFY (m_Desc->m_Images[WallpaperDescriptor::Landscape].m_Pixmap.width() == 0);
    QVERIFY (m_Desc->m_Images[WallpaperDescriptor::Landscape].m_Pixmap.height() == 0);

    dropDescriptor ();
}

void
Ft_WallpaperDescriptor::testThumbnailing ()
{
    createDescriptor ();
    m_SignalSink.reset ();

    QFile file (defaultLandscapeImageFile);
    if (!file.exists()) {
        SYS_WARNING (
"File %s does not exists. This test is aborted and a success will be reported."
"I have no idea what is happening with this file!!", 
        SYS_STR(defaultLandscapeImageFile));
        return;
    }

    /*
     * Let's see if the desscriptor does initiate the thumbnailing when a proper
     * url and mimetype is set.
     */
    m_Desc->setFilename (defaultLandscapeImageFile);
    m_Desc->setMimeType (defaultLandscapeMimeType);
    m_Desc->initiateThumbnailer ();

    // We can not check this, because the thumbnailer might be faster than us.
    // QVERIFY (m_Desc->m_Thumbnailer != 0);

    /*
     * We are prepared to wait let's say 10 seconds for the thumbnail to get
     * ready, but sometimes we don't have to wait that much. Especially under
     * scratchbox when the test is executed the tenth times.
     */
    for (int i = 0; i < 10; ++i) {
        if (m_SignalSink.m_ThumbnailLoaded)
            break;

        QTest::qWait (1000);
    }

    QVERIFY (m_Desc->isThumbnailLoaded());
    QVERIFY (m_SignalSink.m_ThumbnailLoaded);
    QVERIFY (m_Desc->m_Thumbnailer == 0);

    dropDescriptor ();    
}

/******************************************************************************
 * Low lever helper functions.
 */
void
Ft_WallpaperDescriptor::dropDescriptor ()
{
    if (m_Desc) {
        delete m_Desc;
        m_Desc = 0;
    }
}

void
Ft_WallpaperDescriptor::createDescriptor ()
{
    bool                 connectSuccess;
    
    if (m_Desc)
        delete m_Desc;

    m_Desc = new WallpaperDescriptor;
    /*
     * Checking if the signals are there and of course connecting to them.
     */
    connectSuccess = connect (
            m_Desc, SIGNAL (thumbnailLoaded (WallpaperDescriptor *)),
            &m_SignalSink, SLOT (thumbnailLoaded (WallpaperDescriptor *)));
    QVERIFY (connectSuccess);
}

QTEST_APPLESS_MAIN(Ft_WallpaperDescriptor)




