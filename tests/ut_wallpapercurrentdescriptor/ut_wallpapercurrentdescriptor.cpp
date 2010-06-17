/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#include "ut_wallpapercurrentdescriptor.h"
#include "mdesktopentrystub.h"
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

    success = m_Desc->setFromDestopFile (WALLPAPER_DESKTOPFILE_MISSING);
    QVERIFY (!success);
    QVERIFY (!m_Desc->valid());
    QVERIFY (!m_Desc->m_DesktopEntry);

    dropDescriptor ();
}

void
Ut_WallpaperCurrentDescriptor::testFromFileNames ()
{
    QString file1 ("nolandscapefile.png");
    QString file2 ("noportraitfile.png");
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

    QString desktopString = curr->generateDesktopFile ("ThisIsThePath");
    SYS_DEBUG ("*** = %s", SYS_STR(desktopString));
    // FIXME: We obviously should do a much 

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

QTEST_APPLESS_MAIN(Ut_WallpaperCurrentDescriptor)




