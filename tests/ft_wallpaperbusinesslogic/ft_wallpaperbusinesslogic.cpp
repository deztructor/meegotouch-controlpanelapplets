/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#include "ft_wallpaperbusinesslogic.h"

#include "wallpaperbusinesslogic.h"
#include "wallpapercurrentdescriptor.h"
#include "wallpaperitrans.h"

#include <QPixmap>

#include <MApplication>
#include <MGConfItem>

#define DEBUG
#include "../../src/debug.h"

static const QString PortraitKey = 
    "/desktop/meego/background/portrait/picture_filename";
static const QString LandscapeKey = 
    "/desktop/meego/background/landscape/picture_filename";

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
    m_WallpaperChangedCame = false;
}

void 
SignalSink::wallpaperChanged ()
{
    SYS_DEBUG ("");
    m_WallpaperChangedCame = true;
}

/******************************************************************************
 * Ft_WallpaperBusinessLogic implementation. 
 */
void 
Ft_WallpaperBusinessLogic::init()
{
}

void 
Ft_WallpaperBusinessLogic::cleanup()
{
}


static int argc = 1;
static char* app_name = (char*) "./Ft_WallpaperBusinessLogic";

void 
Ft_WallpaperBusinessLogic::initTestCase()
{
    bool connectSuccess;

    m_App = new MApplication (argc, &app_name);
    m_Api = new WallpaperBusinessLogic;

    connectSuccess = connect (
            m_Api, SIGNAL (wallpaperChanged()),
            &m_SignalSink, SLOT (wallpaperChanged()));
    QVERIFY (connectSuccess);
}

void 
Ft_WallpaperBusinessLogic::cleanupTestCase()
{
    delete m_App;
    delete m_Api;
}

void
Ft_WallpaperBusinessLogic::testCreateDirectory ()
{
    QString path;

    path = m_Api->dirPath();
    m_Api->ensureHasDirectory ();

    QDir outputDir (path);
    QVERIFY (outputDir.exists());
}


/*!
 * Gets the list of the available wallpapers, checks if the list is not empty,
 * then goes through the potential wallpapers and checks if all of them has the
 * string type data available. These strings (filename, basename, and so on)
 * should be available for all offered wallpapers.
 */
void
Ft_WallpaperBusinessLogic::testAvailableWallpapers ()
{
    QList<WallpaperDescriptor *> availableWallpapers;
    int n;

    availableWallpapers = m_Api->availableWallpapers ();
    /*
     * There should be at least one available wallpaper, that is the current
     * wallpaper.
     */
    SYS_DEBUG ("We have %d available wallpapers.", availableWallpapers.size());
    QVERIFY (availableWallpapers.size() > 0);

    n = 0;
    foreach (WallpaperDescriptor *desc, availableWallpapers) {
        QString   filename, title, basename;
        QString   extension, mimetype;
        QString   suggestedp, suggestedl;
        QString   originalp, originall;

        filename = desc->filename ();
        title = desc->title ();
        basename = desc->basename ();
        extension = desc->extension ();
        mimetype = desc->mimeType ();
        suggestedp = desc->suggestedOutputFilename (M::Portrait);
        suggestedl = desc->suggestedOutputFilename (M::Landscape);
        originalp = desc->originalImageFile (M::Portrait);
        originall = desc->originalImageFile (M::Landscape);

        #if 0
        /*
         * These might prove usefull in the future, but obviously generate too
         * much output.
         */
        SYS_DEBUG ("*********** available wallpaper #%3d ****************", n);
        SYS_DEBUG ("*** filename   = %s", SYS_STR(filename));
        SYS_DEBUG ("*** title      = %s", SYS_STR(title));
        SYS_DEBUG ("*** basename   = %s", SYS_STR(basename));
        SYS_DEBUG ("*** mimetype   = %s", SYS_STR(mimetype));
        SYS_DEBUG ("*** extension  = %s", SYS_STR(extension));
        SYS_DEBUG ("*** suggestedp = %s", SYS_STR(suggestedp));
        SYS_DEBUG ("*** suggestedl = %s", SYS_STR(suggestedl));
        SYS_DEBUG ("*** originalp  = %s", SYS_STR(originalp));
        SYS_DEBUG ("*** originall  = %s", SYS_STR(originall));
        #endif
        QVERIFY (!filename.isEmpty());
        QVERIFY (!title.isEmpty());
        QVERIFY (!basename.isEmpty());
        QVERIFY (!mimetype.isEmpty());
        QVERIFY (!suggestedp.isEmpty());
        QVERIFY (!suggestedl.isEmpty());
        QVERIFY (!originalp.isEmpty());
        QVERIFY (!originall.isEmpty());

        ++n;
    }
}

/*!
 * Checks if the current wallpaper descriptor is available and it is valid, it
 * presents all the string type properties.
 */
void
Ft_WallpaperBusinessLogic::testCurrentWallpaper ()
{
    WallpaperCurrentDescriptor *desc = WallpaperCurrentDescriptor::instance ();
    QString   filename, title, basename;
    QString   extension, mimetype;
    QString   suggestedp, suggestedl;
    QString   originalp, originall;

    QVERIFY (desc != 0);

    filename = desc->filename ();
    title = desc->title ();
    basename = desc->basename ();
    extension = desc->extension ();
    mimetype = desc->mimeType ();
    suggestedp = desc->suggestedOutputFilename (M::Portrait);
    suggestedl = desc->suggestedOutputFilename (M::Landscape);
    originalp = desc->originalImageFile (M::Portrait);
    originall = desc->originalImageFile (M::Landscape);

    #if 0
    /*
     * These might prove usefull in the future, but obviously generate too much
     * output.
     */
    SYS_DEBUG ("*********** current wallpaper ***********************");
    SYS_DEBUG ("*** filename   = %s", SYS_STR(filename));
    SYS_DEBUG ("*** title      = %s", SYS_STR(title));
    SYS_DEBUG ("*** basename   = %s", SYS_STR(basename));
    SYS_DEBUG ("*** mimetype   = %s", SYS_STR(mimetype));
    SYS_DEBUG ("*** extension  = %s", SYS_STR(extension));
    SYS_DEBUG ("*** suggestedp = %s", SYS_STR(suggestedp));
    SYS_DEBUG ("*** suggestedl = %s", SYS_STR(suggestedl));
    SYS_DEBUG ("*** originalp  = %s", SYS_STR(originalp));
    SYS_DEBUG ("*** originall  = %s", SYS_STR(originall));
    #endif

    QVERIFY (!filename.isEmpty());
    QVERIFY (!title.isEmpty());
    QVERIFY (!basename.isEmpty());
    QVERIFY (!mimetype.isEmpty());
    QVERIFY (!suggestedp.isEmpty());
    QVERIFY (!suggestedl.isEmpty());
    QVERIFY (!originalp.isEmpty());
    QVERIFY (!originall.isEmpty());

    QVERIFY (desc->isCurrent());
    QVERIFY (desc->valid());
}


/*!
 * This function will try to find the first available wallpaper that is not the
 * current wallpaper and will set it with various image transformations. The
 * saved images will be loaded to test the availablity and the size.
 */
void
Ft_WallpaperBusinessLogic::testSetWallpapert ()
{
    WallpaperITrans landscapeITrans;
    WallpaperITrans portraitITrans;
    QList<WallpaperDescriptor *> availableWallpapers;
    int n;

    availableWallpapers = m_Api->availableWallpapers ();
    for (n = 0; n < availableWallpapers.size(); ++n) {
        if (!availableWallpapers[n]->isCurrent())
            break;
    }

    SYS_DEBUG ("*** n = %d", n);
    if (n == availableWallpapers.size()) {
        SYS_WARNING ("Only one image?");
        return;
    }

    /*
     * Testing with scale and offset set to the default.
     */
    // FIXME: setExpectedSize? This should not be needed...
    landscapeITrans.setExpectedSize (QSize(864, 480));
    portraitITrans.setExpectedSize (QSize(480, 864));
    m_SignalSink.reset ();
    m_Api->setBackground (
            &landscapeITrans,
            &portraitITrans,
             availableWallpapers[n]);

    // Testing if the images are valid and we got a signal about the change.
    testValidImages ();
    QVERIFY (m_SignalSink.m_WallpaperChangedCame);

    /*
     * Testing with some arbitrary scale and offset images.
     */
    landscapeITrans.setScale (0.3);
    portraitITrans.setScale (0.2);   
    landscapeITrans.setOffset (QPointF(10, 20));
    portraitITrans.setOffset (QPointF(20, 10));

    m_SignalSink.reset ();
    m_Api->setBackground (
            &landscapeITrans,
            &portraitITrans,
             availableWallpapers[n]);
    
    // Testing if the images are valid and we got a signal about the change.
    testValidImages ();
    QVERIFY (m_SignalSink.m_WallpaperChangedCame);
    
    /*
     * Also with magnifying and negative offsets.
     */
    landscapeITrans.setScale (1.5);
    portraitITrans.setScale (1.3);   
    landscapeITrans.setOffset (QPointF(-10, -200));
    portraitITrans.setOffset (QPointF(-800, -300));

    m_SignalSink.reset ();
    m_Api->setBackground (
            &landscapeITrans,
            &portraitITrans,
             availableWallpapers[n]);
    
    // Testing if the images are valid and we got a signal about the change.
    testValidImages ();
    QVERIFY (m_SignalSink.m_WallpaperChangedCame);
}

/******************************************************************************
 * Private functions.
 */

/*!
 * This low level function will check the GConf database, read the image file
 * names and see if it is possible to load them. If the images can be loaded the
 * function will test if the image sizes are correct.
 *
 * This test function whould be executed whenever the test program sets a new
 * wallpaper.
 */
void
Ft_WallpaperBusinessLogic::testValidImages ()
{
    MGConfItem   *landscapeGConfItem;
    MGConfItem   *portraitGConfItem;
    QString       landscapeFile;
    QString       portraitFile;
    QPixmap       pixmap;
    bool          success;

    landscapeGConfItem = new MGConfItem (LandscapeKey);
    portraitGConfItem = new MGConfItem (PortraitKey);

    landscapeFile = landscapeGConfItem->value().toString();
    portraitFile = portraitGConfItem->value().toString();

    SYS_DEBUG ("*** landscapeFile = %s", SYS_STR(landscapeFile));
    SYS_DEBUG ("*** portraitFile  = %s", SYS_STR(portraitFile));

    QVERIFY (!landscapeFile.isEmpty());
    QVERIFY (!portraitFile.isEmpty());

    success = pixmap.load (landscapeFile);
    QVERIFY (success);
    SYS_DEBUG ("*** landscape size = %dx%d", pixmap.width(), pixmap.height());
    QVERIFY (pixmap.width() == 864);
    QVERIFY (pixmap.height() == 480);

    success = pixmap.load (portraitFile);
    QVERIFY (success);
    SYS_DEBUG ("*** landscape size = %dx%d", pixmap.width(), pixmap.height());
    QVERIFY (pixmap.width() == 480);
    QVERIFY (pixmap.height() == 864);


    delete landscapeGConfItem;
    delete portraitGConfItem;
}

QTEST_APPLESS_MAIN(Ft_WallpaperBusinessLogic)


