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
#if 0
static bool pixmapLoadSuccess = true;

bool 	
QPixmap::load (
        const QString & fileName, 
        const char * format, 
        Qt::ImageConversionFlags flags)
{
    Q_UNUSED (fileName);
    Q_UNUSED (format);
    Q_UNUSED (flags);
    SYS_DEBUG ("*** fileName = %s", SYS_STR(fileName));
    if (pixmapLoadSuccess)
        *this = QPixmap (100, 100);

    return pixmapLoadSuccess;
}

#include <QImage>
bool
QImage::load (
        const QString &fileName, 
        const char     *format)
{
    Q_UNUSED (fileName);
    Q_UNUSED (format);
    SYS_WARNING ("Emulating load of %s", SYS_STR(fileName));
    if (pixmapLoadSuccess) {
        *this = QImage (QSize(864, 480), QImage::Format_RGB16);
        this->fill (0);
    }

    return pixmapLoadSuccess;
}
#endif



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
    WallpaperDescriptor desc;

    QVERIFY (desc.filePath().isEmpty());
    QVERIFY (desc.originalFilePath().isEmpty());
    QVERIFY (!desc.thumbnailPending());
    QVERIFY (!desc.hasThumbnail());
    QVERIFY (!desc.selected());
    QVERIFY (!desc.progress());
    QVERIFY (desc.historyIndex() == -1);
}

/*
 * Tests all but the default constructor. The default constructor is used all
 * over the place and tested also in testDefaults().
 */
void
Ut_WallpaperDescriptor::testConstructors ()
{
#if 0
    /*
     * Testing the constructor that gets one filename.
     */
    WallpaperDescriptor desc1 ("/nodir/NoSuchFile.png");
    
    QVERIFY (desc1.m_Images[WallpaperDescriptor::Portrait].filename() == "/nodir/NoSuchFile.png");
    QVERIFY (desc1.m_Images[WallpaperDescriptor::Portrait].basename() == "NoSuchFile");
    QVERIFY (desc1.m_Images[WallpaperDescriptor::Portrait].extension() == "png");
    QVERIFY (desc1.m_Images[WallpaperDescriptor::Portrait].m_Url.toString() == "file:///nodir/NoSuchFile.png");

    /*
     * Testing the copy constructor.
     */
    WallpaperDescriptor desc2 (desc1);

    QVERIFY (desc2.m_Images[WallpaperDescriptor::Portrait].filename() == "/nodir/NoSuchFile.png");
    QVERIFY (desc2.m_Images[WallpaperDescriptor::Portrait].basename() == "NoSuchFile");
    QVERIFY (desc2.m_Images[WallpaperDescriptor::Portrait].extension() == "png");
    QVERIFY (desc2.m_Images[WallpaperDescriptor::Portrait].m_Url.toString() == "file:///nodir/NoSuchFile.png");
#endif
}

/*!
 * Checking if the descriptor is created from a filename the various filename
 * properties are properly returned.
 */
void
Ut_WallpaperDescriptor::testFilenames ()
{
#if 0
    const QString myFilename ("/nodir/NoSuchFile.png");

    WallpaperDescriptor desc (myFilename);
    QString original  = desc.originalImageFile(M::Portrait);
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
#endif
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
#if 0
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
#ifndef THUMBNAILER_SINGLETON
    QVERIFY (m_Desc->m_Thumbnailer == 0);
#endif

    dropDescriptor ();    
#endif
}

/*
 * Tests the thumbnailing using a local file name and the mime type.
 */
void
Ut_WallpaperDescriptor::testThumbnailingByFilename ()
{
#if 0
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
#ifndef THUMBNAILER_SINGLETON
    QVERIFY (m_Desc->m_Thumbnailer == 0);
#endif

    dropDescriptor ();    
#endif
}

/*!
 * When the descriptor has no filename/url and/or mimetype the thumbnailing
 * should not be initiated.
 */
void
Ut_WallpaperDescriptor::testThumbnailingWithoutData ()
{
#if 0
    createDescriptor ();
    m_SignalSink.reset ();

    /*
     * Let's see if the desscriptor does not initiate the thumbnailing when the
     * filename and the url are missing.
     */
    m_Desc->initiateThumbnailer ();

#ifndef THUMBNAILER_SINGLETON
    QVERIFY (m_Desc->m_Thumbnailer == 0);
#endif
    QVERIFY (!m_Desc->isThumbnailLoaded());
    QVERIFY (!m_SignalSink.m_ThumbnailLoaded);

    dropDescriptor ();    
#endif
}

/*
 * When the thumbnailer fails the descriptor should destroy the thumbnailer and
 * should stay in a state where it has no thumbnail. Also the descriptor should
 * not emit a signal about the thumbnail load finished.
 */
void
Ut_WallpaperDescriptor::testThumbnailingFailure ()
{
#if 0
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
    QVERIFY (m_Desc->m_Images[WallpaperDescriptor::Portrait].m_MimeType == "image/png");
    QVERIFY (m_Desc->m_Images[WallpaperDescriptor::Portrait].mimeType() == "image/png");

    /*
     * Ok, now we emulate the case when the thumbnail creation is finished with
     * an error. We have to make the QImage loader stub simulate failure,
     * otherwise the descriptor does the thumbnailing by itself.
     */
    pixmapLoadSuccess = false;
    m_Desc->m_Thumbnailer->sendError ();
    m_Desc->m_Thumbnailer->sendFinished ();

    QVERIFY (!m_Desc->isThumbnailLoaded());
    QVERIFY (!m_SignalSink.m_ThumbnailLoaded);
#ifndef THUMBNAILER_SINGLETON
    QVERIFY (m_Desc->m_Thumbnailer == 0);
#endif
    pixmapLoadSuccess = true;

    dropDescriptor ();    
#endif
}
void
Ut_WallpaperDescriptor::testCache ()
{
#if 0
    createDescriptor ();
    m_Desc->setUrl ("file:///simulatedexistingfile.png");

    /*
     * Caching with an existing image file from the main thread...
     */
    pixmapLoadSuccess = true;
    m_Desc->cache();
    QVERIFY (m_Desc->m_Images[WallpaperDescriptor::Portrait].m_Cached);

    /*
     * We have added some other combinations...
     */

    /*
     * Uncaching...
     */
    m_Desc->unCache();
    QVERIFY (!m_Desc->m_Images[WallpaperDescriptor::Portrait].m_Cached);

    dropDescriptor ();
#endif
}


/******************************************************************************
 * Low level test programs.
 */
void
Ut_WallpaperDescriptor::dropDescriptor ()
{
#if 0
    if (m_Desc) {
        delete m_Desc;
        m_Desc = 0;
    }
#endif
}

void
Ut_WallpaperDescriptor::createDescriptor ()
{
#if 0
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
#endif
}

QTEST_APPLESS_MAIN(Ut_WallpaperDescriptor)



