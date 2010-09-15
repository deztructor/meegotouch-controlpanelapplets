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

#include "ut_wallpaperdescriptor.h"

#include "wallpaperdescriptor.h"
#include "wallpaperitrans.h"

#include <QPixmap>
#include <QUrl>
#include <QString>

#include <MApplication>
#include <MGConfItem>

#define DEBUG
#include "../../src/debug.h"

/******************************************************************************
 * 
 */
static bool pixmapLoadSuccess = true;

bool 	
QPixmap::load (
        const QString & fileName, 
        const char * format, 
        Qt::ImageConversionFlags flags)
{
    Q_UNUSED (format);
    Q_UNUSED (flags);
    SYS_DEBUG ("*** fileName = %s", SYS_STR(fileName));
    if (pixmapLoadSuccess)
        *this = QPixmap (100, 100);

    return pixmapLoadSuccess;
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
 * Ut_WallpaperDescriptor implementation. 
 */
void 
Ut_WallpaperDescriptor::init()
{
}

void 
Ut_WallpaperDescriptor::cleanup()
{
}


static int argc = 1;
static char* app_name = (char*) "./Ut_WallpaperDescriptor";

void 
Ut_WallpaperDescriptor::initTestCase()
{
    m_App = new MApplication (argc, &app_name);
    m_Desc = 0;
}

void 
Ut_WallpaperDescriptor::cleanupTestCase()
{
    if (m_Desc)
        delete m_Desc;

    m_App->deleteLater ();
}

/*!
 * Checking if the newly created descriptor has the proper default values.
 */
void 
Ut_WallpaperDescriptor::testDefaults ()
{
    createDescriptor ();

    QVERIFY (m_Desc->m_Thumbnailer == NULL);
    QVERIFY (m_Desc->m_Images[WallpaperDescriptor::Landscape].m_Filename.isEmpty());
    QVERIFY (m_Desc->m_Images[WallpaperDescriptor::Landscape].m_Title.isEmpty());
    QVERIFY (m_Desc->m_Images[WallpaperDescriptor::Landscape].m_MimeType.isEmpty());
    QVERIFY (m_Desc->m_Images[WallpaperDescriptor::Landscape].m_HasThumbnail == false);
    QVERIFY (m_Desc->m_Images[WallpaperDescriptor::Landscape].m_Cached == false);
    QVERIFY (m_Desc->isCurrent() == false);
    QVERIFY (m_Desc->version() == 0);

    /*
     * This function always return true, only the inherited
     * WallpaperCurrentDescriptor has tests in it.
     */
    QVERIFY (m_Desc->valid());

    dropDescriptor ();    
}

/*
 * Tests all but the default constructor. The default constructor is used all
 * over the place and tested also in testDefaults().
 */
void
Ut_WallpaperDescriptor::testConstructors ()
{
    /*
     * Testing the constructor that gets one filename.
     */
    WallpaperDescriptor desc1 ("/nodir/NoSuchFile.png");
    
    QVERIFY (desc1.m_Images[WallpaperDescriptor::Landscape].filename() == "/nodir/NoSuchFile.png");
    QVERIFY (desc1.m_Images[WallpaperDescriptor::Landscape].basename() == "NoSuchFile");
    QVERIFY (desc1.m_Images[WallpaperDescriptor::Landscape].extension() == "png");
    QVERIFY (desc1.m_Images[WallpaperDescriptor::Landscape].m_Url.toString() == "file:///nodir/NoSuchFile.png");

    /*
     * Testing the copy constructor.
     */
    WallpaperDescriptor desc2 (desc1);

    QVERIFY (desc2.m_Images[WallpaperDescriptor::Landscape].filename() == "/nodir/NoSuchFile.png");
    QVERIFY (desc2.m_Images[WallpaperDescriptor::Landscape].basename() == "NoSuchFile");
    QVERIFY (desc2.m_Images[WallpaperDescriptor::Landscape].extension() == "png");
    QVERIFY (desc2.m_Images[WallpaperDescriptor::Landscape].m_Url.toString() == "file:///nodir/NoSuchFile.png");
}

/*!
 * Checking if the descriptor is created from a filename the various filename
 * properties are properly returned.
 */
void
Ut_WallpaperDescriptor::testFilenames ()
{
    const QString myFilename ("/nodir/NoSuchFile.png");

    WallpaperDescriptor desc (myFilename);
    QString original  = desc.originalImageFile(M::Landscape);
    QString portrait  = desc.suggestedOutputFilename(M::Portrait);
    QString landscape = desc.suggestedOutputFilename(M::Landscape);

    if (original != myFilename) {
        SYS_WARNING ("originalImageFile(M::Landscape) should return '%s' "
                "but it returns '%s'",
                SYS_STR(myFilename),
                SYS_STR(original));
    }

    QVERIFY (original == myFilename);
    QVERIFY (!portrait.isEmpty());
    QVERIFY (!landscape.isEmpty());
    QVERIFY (!portrait.contains("/"));
    QVERIFY (!landscape.contains("/"));
    
    // Now this is important!
    QVERIFY (landscape != portrait);
}

/*!
 * When the descriptor has an url and a mime type it should be able to initiate
 * the thumbnailing and when the thumbnailing is successfull the descriptor
 * should report by emitting the thumbnailLoaded() signal and should have the
 * proper thumbnail.
 *
 * In this case the thumbnail is created by the Thumbnailer stub so there is no
 * file activity of any kind.
 */
void
Ut_WallpaperDescriptor::testThumbnailing ()
{
    createDescriptor ();
    m_SignalSink.reset ();

    /*
     * Let's see if the desscriptor does initiate the thumbnailing when a proper
     * url and mimetype is set.
     */
    m_Desc->setUrl ("file:///NoSuchAFile.png");
    m_Desc->setMimeType ("image/png");
    m_Desc->initiateThumbnailer ();

    QVERIFY (m_Desc->m_Thumbnailer != 0);
    QVERIFY (m_Desc->m_Thumbnailer->m_RequestCame);

    /*
     * Ok, now we emulate the case when the thumbnail creation is finished.
     */
    m_Desc->m_Thumbnailer->sendThumbnail ();
    m_Desc->m_Thumbnailer->sendFinished ();

    QVERIFY (m_Desc->isThumbnailLoaded());
    QVERIFY (m_SignalSink.m_ThumbnailLoaded);
    QVERIFY (m_Desc->m_Thumbnailer == 0);

    /*
     * This is actually a different subject, but we can test the title here.
     */
    QVERIFY (m_Desc->title() == "NoSuchAFile");
    m_Desc->setTitle ("The Title");
    QVERIFY (m_Desc->title() == "The Title");

    dropDescriptor ();    
}

/*
 * Tests the thumbnailing using a local file name and the mime type.
 */
void
Ut_WallpaperDescriptor::testThumbnailingByFilename ()
{
    createDescriptor ();
    m_SignalSink.reset ();

    /*
     * Let's see if the desscriptor does initiate the thumbnailing when a proper
     * url and mimetype is set.
     */
    m_Desc->setFilename ("/nodirectory/NoSuchAFile.png");
    m_Desc->setMimeType ("image/png");
    m_Desc->initiateThumbnailer ();

    QVERIFY (m_Desc->m_Thumbnailer != 0);
    QVERIFY (m_Desc->m_Thumbnailer->m_RequestCame);

    /*
     * Ok, now we emulate the case when the thumbnail creation is finished.
     */
    m_Desc->m_Thumbnailer->sendThumbnail ();
    m_Desc->m_Thumbnailer->sendFinished ();

    QVERIFY (m_Desc->isThumbnailLoaded());
    QVERIFY (m_SignalSink.m_ThumbnailLoaded);
    QVERIFY (m_Desc->m_Thumbnailer == 0);

    dropDescriptor ();    
}

/*!
 * When the descriptor has no filename/url and/or mimetype the thumbnailing
 * should not be initiated.
 */
void
Ut_WallpaperDescriptor::testThumbnailingWithoutData ()
{
    createDescriptor ();
    m_SignalSink.reset ();

    /*
     * Let's see if the desscriptor does not initiate the thumbnailing when the
     * filename and the url are missing.
     */
    m_Desc->initiateThumbnailer ();

    QVERIFY (m_Desc->m_Thumbnailer == 0);
    QVERIFY (!m_Desc->isThumbnailLoaded());
    QVERIFY (!m_SignalSink.m_ThumbnailLoaded);

    dropDescriptor ();    
}

/*
 * When the thumbnailer fails the descriptor should destroy the thumbnailer and
 * should stay in a state where it has no thumbnail. Also the descriptor should
 * not emit a signal about the thumbnail load finished.
 */
void
Ut_WallpaperDescriptor::testThumbnailingFailure ()
{
    createDescriptor ();
    m_SignalSink.reset ();

    /*
     * Let's see if the desscriptor does initiate the thumbnailing when a proper
     * url and mimetype is set.
     */
    m_Desc->setUrl ("file:///NoSuchAFile.png");
    m_Desc->setMimeType ("image/png");
    m_Desc->initiateThumbnailer ();

    QVERIFY (m_Desc->m_Thumbnailer != 0);
    QVERIFY (m_Desc->m_Thumbnailer->m_RequestCame);
    QVERIFY (m_Desc->m_Images[WallpaperDescriptor::Landscape].m_MimeType == "image/png");
    QVERIFY (m_Desc->m_Images[WallpaperDescriptor::Landscape].mimeType() == "image/png");

    /*
     * Ok, now we emulate the case when the thumbnail creation is finished with
     * an error. We have to make the pixmap loader stub simulate failure,
     * otherwise the descriptor does the thumbnailing by itself.
     */
    pixmapLoadSuccess = false;
    m_Desc->m_Thumbnailer->sendError ();
    m_Desc->m_Thumbnailer->sendFinished ();

    QVERIFY (!m_Desc->isThumbnailLoaded());
    QVERIFY (!m_SignalSink.m_ThumbnailLoaded);
    QVERIFY (m_Desc->m_Thumbnailer == 0);
    
    pixmapLoadSuccess = true;

    dropDescriptor ();    
}
void
Ut_WallpaperDescriptor::testCache ()
{
    createDescriptor ();
    m_Desc->setUrl ("file:///simulatedexistingfile.png");

    /*
     * Caching with an existing image file from the main thread...
     */
    pixmapLoadSuccess = true;
    m_Desc->cache();
    QVERIFY (m_Desc->m_Images[WallpaperDescriptor::Landscape].m_Cached);

    /*
     * We have added some other combinations...
     */

    /*
     * Uncaching...
     */
    m_Desc->unCache();
    QVERIFY (!m_Desc->m_Images[WallpaperDescriptor::Landscape].m_Cached);

    dropDescriptor ();
}


/******************************************************************************
 * Low level test programs.
 */
void
Ut_WallpaperDescriptor::dropDescriptor ()
{
    if (m_Desc) {
        delete m_Desc;
        m_Desc = 0;
    }
}

void
Ut_WallpaperDescriptor::createDescriptor ()
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

QTEST_APPLESS_MAIN(Ut_WallpaperDescriptor)



