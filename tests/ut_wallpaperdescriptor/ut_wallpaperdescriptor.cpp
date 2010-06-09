/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#include "ut_wallpaperdescriptor.h"

#include "wallpaperdescriptor.h"
#include "wallpapercurrentdescriptor.h"
#include "wallpaperitrans.h"

#include <QPixmap>
#include <QUrl>
#include <QString>

#include <MApplication>
#include <MGConfItem>

#define DEBUG
#include "../../src/debug.h"


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

void 
Ut_WallpaperDescriptor::testDefaults ()
{
    createDescriptor ();
}

void
Ut_WallpaperDescriptor::testThumbnailing ()
{
    createDescriptor ();
    // This migh t be a bug!
    //m_Desc->setFilename ("NoSuchAFile.png");

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



