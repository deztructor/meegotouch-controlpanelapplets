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

#include "ut_wallpapercurrentdescriptor.h"
#include "mdesktopentry.h"
#include "wallpapercurrentdescriptor.h"
#include "wallpaperitrans.h"
#include "wallpapergconf.h"

#include <QPixmap>
#include <QUrl>
#include <QString>

#include <MApplication>
#include <MGConfItem>

#define DEBUG
#include "../../src/debug.h"


QVariant
MGConfItem::value () const
{
    QString retval;

    SYS_DEBUG ("*** key() = %s", SYS_STR(key()));

    if (key() == WALLPAPER_LANDSCAPE_KEY) {
        retval = WALLPAPER_LANDSCAPE_EDITEDFILE;
        goto return_string;
    }

    if (key() == WALLPAPER_PORTRAIT_KEY) {
        retval = WALLPAPER_PORTRAIT_EDITEDFILE;
        goto return_string;
    }

    return QVariant();

return_string:
    return QVariant (retval);
}

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
    SYS_DEBUG ("");

    m_ThumbnailLoaded = true;
}

/******************************************************************************
 * Ut_WallpaperCurrentDescriptor implementation. 
 */
void 
Ut_WallpaperCurrentDescriptor::init()
{
}

void 
Ut_WallpaperCurrentDescriptor::cleanup()
{
}


static int argc = 1;
static char* app_name = (char*) "./Ut_WallpaperCurrentDescriptor";

void 
Ut_WallpaperCurrentDescriptor::initTestCase()
{
    m_App = new MApplication (argc, &app_name);
    m_Desc = 0;
}

void 
Ut_WallpaperCurrentDescriptor::cleanupTestCase()
{
    if (m_Desc)
        delete m_Desc;

    m_App->deleteLater ();
}

/*!
 * This test will create a current descriptor and check if the default values
 * has been set.
 */
void
Ut_WallpaperCurrentDescriptor::testDefaults ()
{
    createDescriptor ();
    QVERIFY (m_Desc->isCurrent());
    dropDescriptor ();
}

/*
 * This test will check what happens when we try to initialize the current
 * descriptor using a desktop file that does not exists or the MdesktopEntry
 * reports an invalid file format.
 *
 * Please note that the MDesktopEntry is stubbed, so there will be no file
 * activity.
 */
void
Ut_WallpaperCurrentDescriptor::testFromDesktopFileInvalid ()
{
    bool success;

    createDescriptor ();

    success = m_Desc->setFromDesktopFile (WALLPAPER_DESKTOPFILE_MISSING);
    QVERIFY (!success);
    QVERIFY (!m_Desc->valid());
    QVERIFY (!m_Desc->m_DesktopEntry);

    dropDescriptor ();
}

void
Ut_WallpaperCurrentDescriptor::testFromDesktopFile ()
{
    bool success;

    createDescriptor ();

    success = m_Desc->setFromDesktopFile (wallpaperDesktopFileName());
    QVERIFY (success);
    QVERIFY (m_Desc->valid());
    QVERIFY (!m_Desc->m_DesktopEntry);
    
    QString originalPortrait  = m_Desc->originalImageFile (M::Portrait);
    QString originalLandscape = m_Desc->originalImageFile (M::Landscape);
    QVERIFY (originalPortrait == WALLPAPER_PORTRAIT_ORIGINALFILE);
    QVERIFY (originalLandscape == WALLPAPER_LANDSCAPE_ORIGINALFILE);
    
    QString editedPortrait = m_Desc->editedFilename (M::Portrait);
    QString editedLandscape = m_Desc->editedFilename (M::Landscape);
    QVERIFY (editedPortrait == WALLPAPER_PORTRAIT_EDITEDFILE);
    QVERIFY (editedLandscape == WALLPAPER_LANDSCAPE_EDITEDFILE);
    
    /*
     * We could add more checks here, we have some other fields...
     */


    dropDescriptor ();
}

void
Ut_WallpaperCurrentDescriptor::testFromFileNames ()
{
    QString file1 ("/nodir/nolandscapefile.png");
    QString file2 ("/nodir/noportraitfile.png");
    WallpaperCurrentDescriptor *curr = WallpaperCurrentDescriptor::instance();

    curr->setFromFilenames (file1, file2);

    /*
     * When set from filenames all these properties should have the same value.
     * This is the situation when the current wallpaper is set from the gconf
     * items.
     */
    QString originalPortrait  = curr->originalImageFile (M::Portrait);
    QString originalLandscape = curr->originalImageFile (M::Landscape);
    QVERIFY (originalPortrait == file2);
    QVERIFY (originalLandscape == file1);

    QString suggestedPortrait = curr->suggestedOutputFilename (M::Portrait);
    QString suggestedLandscape = curr->suggestedOutputFilename (M::Landscape);
    QVERIFY (suggestedPortrait != suggestedLandscape);
    QVERIFY (!suggestedPortrait.contains("/"));
    QVERIFY (!suggestedLandscape.contains("/"));
    
    QString editedPortrait = curr->editedFilename (M::Portrait);
    QString editedLandscape = curr->editedFilename (M::Landscape);
    SYS_DEBUG ("*** = %s", SYS_STR(editedPortrait));
    SYS_DEBUG ("*** = %s", SYS_STR(editedLandscape));
    QVERIFY (editedPortrait == file2);
    QVERIFY (editedLandscape == file1);

    /*
     * When set from image file names the image transformations should be all
     * default.
     */
    WallpaperITrans trans1 = curr->iTrans (M::Portrait);
    WallpaperITrans trans2 = curr->iTrans (M::Landscape);

    QVERIFY (trans1.scale() == 1.0);
    QVERIFY (trans1.x() == 0);
    QVERIFY (trans1.y() == 0);
    
    QVERIFY (trans2.scale() == 1.0);
    QVERIFY (trans2.x() == 0);
    QVERIFY (trans2.y() == 0);
    
    dropDescriptor ();
}

/******************************************************************************
 * Low level test programs.
 */
void
Ut_WallpaperCurrentDescriptor::dropDescriptor ()
{
    if (m_Desc) {
        delete m_Desc;
        m_Desc = 0;
    }
}

void
Ut_WallpaperCurrentDescriptor::createDescriptor ()
{
    bool                 connectSuccess;
    
    if (m_Desc)
        delete m_Desc;

    m_Desc = WallpaperCurrentDescriptor::instance();
    /*
     * Checking if the signals are there and of course connecting to them.
     */
    connectSuccess = connect (
            m_Desc, SIGNAL (thumbnailLoaded (WallpaperDescriptor *)),
            &m_SignalSink, SLOT (thumbnailLoaded (WallpaperDescriptor *)));
    QVERIFY (connectSuccess);

    /*
     * Checking if the slots are there.
     */
    connectSuccess = connect (
            &m_SignalSink, SIGNAL(thumbnailReady(QUrl, QUrl, QPixmap, QString)),
            m_Desc, SLOT (thumbnailReady(QUrl, QUrl, QPixmap, QString)));
    QVERIFY (connectSuccess);
    
    connectSuccess = connect (
            &m_SignalSink, SIGNAL(thumbnailError(QString, QUrl)),
            m_Desc, SLOT (thumbnailError(QString, QUrl)));
    QVERIFY (connectSuccess);

    connectSuccess = connect (
            &m_SignalSink, SIGNAL(thumbnailLoadingFinished(int)),
            m_Desc, SLOT (thumbnailLoadingFinished(int)));
    QVERIFY (connectSuccess);
}

/*
 * Returns the same absolute file name the wallpaperbusinesslogic uses to access
 * the saved desktop file. This file is simulated in the mdesktopentry stub with
 * a perfect content.
 */
QString
Ut_WallpaperCurrentDescriptor::wallpaperDesktopFileName ()
{
    QString retval = 
        QString(getenv("HOME")) + "/.wallpapers/wallpaper.desktop";

    return retval;
}


QTEST_APPLESS_MAIN(Ut_WallpaperCurrentDescriptor)




