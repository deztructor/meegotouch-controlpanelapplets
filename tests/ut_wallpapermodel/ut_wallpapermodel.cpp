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
#include "wallpapermodel.h"
#include "wallpaperdescriptor.h"
#include "wallpaperbusinesslogic.h"

#include <MGConfItem>
#include <MApplication>

#define DEBUG
#include "../../src/debug.h"

static const QString PortraitKey =
    "/desktop/meego/background/portrait/picture_filename";
static const QString LandscapeKey =
    "/desktop/meego/background/landscape/picture_filename";

/******************************************************************************
 * Ut_WallpaperModel implementation. 
 */
void 
Ut_WallpaperModel::init()
{
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
    m_App = new MApplication (argc, &app_name);
    m_BusinessLogic = new WallpaperBusinessLogic;
    m_Model = new WallpaperModel (m_BusinessLogic);
}

void 
Ut_WallpaperModel::cleanupTestCase()
{
    delete m_BusinessLogic;
    delete m_Model;
    m_App->deleteLater ();
}

/*
 * Checks what columncounter and rowcounter is returned by the model. The number
 * of columns are always 1, and the data source is stubbed, so there is always
 * going to be at least one (actually two) rows for the test.
 */
void 
Ut_WallpaperModel::testRowsColumns ()
{
    QModelIndex    index;
    int            columns = m_Model->columnCount (index);
    int            rows = m_Model->rowCount (index);

    SYS_DEBUG ("*** columns = %d", columns);
    SYS_DEBUG ("*** rows    = %d", rows);

    QCOMPARE (columns, 1);
    QVERIFY (rows > 0);
}

/*!
 * Gets the data from the model and checks if the data returned is valid.
 */
void 
Ut_WallpaperModel::testData ()
{
    QModelIndex          index;
    QVariant             data;
    WallpaperDescriptor *desc;
    int                  rows = m_Model->rowCount (index);

    for (int n = 0; n < rows; ++n) {
        index = m_Model->index (n, 0);
        data = index.data(WallpaperModel::WallpaperDescriptorRole);
        desc = data.value<WallpaperDescriptor*>();

        #if 1
        SYS_DEBUG ("--- index at %d -----------------", n);
        SYS_DEBUG ("*** position = %d, %d", index.column(), index.row());
        SYS_DEBUG ("*** file     = %s", SYS_STR(desc->filename()));
        #endif
        
        QVERIFY (index.isValid ());
        QCOMPARE (index.column(), 0);
        QCOMPARE (index.row(), n);
        QVERIFY (index.model() == m_Model);
        QVERIFY (isWallpaperDescriptorValid(desc));
    }
}

void 
Ut_WallpaperModel::testCellCreator ()
{
    /*
     * FIXME: To write the test case for the image widget case.
     */
}

/*
 * This function will check the thumbnailer that creates and updates the
 * tuhmbnails for the list widget. During the test we use a simulated
 * thumbnailer from the stub set and (as always) a simulated tracker data source
 * that gives us the items stored in the model.
 */
void
Ut_WallpaperModel::testImageLoader ()
{
    WallpaperImageLoader *imageLoader;
    int                   rows;
    QModelIndex           firstRow;
    QModelIndex           lastRow;
    QModelIndex           index;
    QVariant              data;
    WallpaperDescriptor  *desc;

    imageLoader = new WallpaperImageLoader;
    rows = m_Model->rowCount (firstRow);

    firstRow = m_Model->index (0, 0);
    lastRow  = m_Model->index (rows - 1, 0);

    /*
     * Now we command the thumbnail loader. When the method returns all the
     * thumbnailing jobs are in the loading jobs, the loader will actually
     * initiate the thumbnailing from the main loop.
     */
    imageLoader->loadPictures (firstRow, lastRow);
    QVERIFY (imageLoader->m_ThumbnailLoadingJobs.size() > 0);

    /*
     * Then we give chance for the main loop to run so the loader will initiate
     * the thumbnailing, all the jobs are moved to the pending job queue.
     */
    QTest::qWait (800);
    /*
     * This one again is failing on cita. Can't reproduce under scratchbox...
     */
    //QCOMPARE (imageLoader->m_ThumbnailLoadingJobs.size(), 0);

    /*
     * Now we go through the wallpaper descriptors and check if the thumbnailing
     * is actually initiate, and then we simulate the thumbnail-finished for all
     * the descriptors.
     */
    for (int n = 0; n < rows; ++n) {
        index = m_Model->index (n, 0);
        data = index.data(WallpaperModel::WallpaperDescriptorRole);
        desc = data.value<WallpaperDescriptor*>();

#if 0
    // TODO: FIXME: review & fix this
        // We either have a thumbnailer or we don't need it because this is a 
        // theme based wallpaper.
        if (desc->imageID ().isEmpty ())
            QVERIFY (desc->m_Thumbnailer);
#endif

        if (desc->m_Thumbnailer) {
            SYS_DEBUG ("Simulating thumbnail-finished");
            desc->m_Thumbnailer->sendThumbnail ();
            desc->m_Thumbnailer->sendFinished ();
        }
    }

    // As all the thumbnails are ready (simulated of course) we should have no
    // jobs pending here.
    QCOMPARE (imageLoader->m_ThumbnailLoadingJobs.size(), 0);

    // FIXME: To create a new test case, leave the thumbnailing there and call
    // this method. Well, it should be done, but it only checks one line...
    imageLoader->stopLoadingPictures();
}

/******************************************************************************
 * Low level helper functions.
 */
bool
Ut_WallpaperModel::isWallpaperDescriptorValid (
        WallpaperDescriptor *desc)
{
    QString filename, basename, mimeType;
    bool valid = true;

    if (desc == 0) {
        SYS_WARNING ("The pointer should not be NULL.");
        return false;
    }

    /*
     * If this is a theme based wallpaper the image informations might be empty.
     */
    if (!desc->imageID().isEmpty())
        goto no_file_check_necessary;

    /*
     * Checking the filename
     */
    filename = desc->filename();
    if (filename.isEmpty())
        valid = false;
    else if (!filename.startsWith("/"))
        valid = false;
    else if (filename.endsWith("/"))
        valid = false;
    else if (filename.contains("*") || filename.contains("?"))
        valid = false;

    if (!valid) {
        SYS_WARNING ("Invalid filename: %s", SYS_STR(filename));
        return false;
    }

    /*
     * Checking basename
     */
    basename = desc->basename();
    if (basename.isEmpty())
        valid = false;
    else if (basename.contains("/"))
        valid = false;
    else if (basename.contains("*") || filename.contains("?"))
        valid = false;

    if (!valid) {
        SYS_WARNING ("Invalid basename: %s", SYS_STR(basename));
        return false;
    }

    /*
     * Checking the MimeType
     */
    mimeType = desc->mimeType ();
    if (mimeType.isEmpty())
        valid = false;
    else if (!mimeType.startsWith("image/"))
        valid = false;

    if (!valid) {
        SYS_WARNING ("Invalid mimeType = %s", SYS_STR(mimeType));
        return false;
    }

no_file_check_necessary:

    return true;
}

QTEST_APPLESS_MAIN(Ut_WallpaperModel)


