/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#include "ut_wallpaperbusinesslogic.h"

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
 * Ut_WallpaperBusinessLogic implementation. 
 */
void 
Ut_WallpaperBusinessLogic::init()
{
}

void 
Ut_WallpaperBusinessLogic::cleanup()
{
}


static int argc = 1;
static char* app_name = (char*) "./Ut_WallpaperBusinessLogic";

void 
Ut_WallpaperBusinessLogic::initTestCase()
{
    m_App = new MApplication (argc, &app_name);
    m_Api = new WallpaperBusinessLogic;
}

void 
Ut_WallpaperBusinessLogic::cleanupTestCase()
{
    delete m_App;
    delete m_Api;
}

/*!
 * Gets the list of the available wallpapers, checks if the list is not empty,
 * then goes through the potential wallpapers and checks if all of them has the
 * string type data available. These strings (filename, basename, and so on)
 * should be available for all offered wallpapers.
 */
void
Ut_WallpaperBusinessLogic::testAvailableWallpapers ()
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
Ut_WallpaperBusinessLogic::testCurrentWallpaper ()
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
 * Checks the low level WallpaperITrans class, its tag methods and overloaded
 * operators.
 */
void
Ut_WallpaperBusinessLogic::testITrans ()
{
    WallpaperITrans trans1, trans2;

    SYS_DEBUG ("Checking default values of WallpaperITrans...");
    QVERIFY (trans1.x() == 0);
    QVERIFY (trans1.y() == 0);
    QVERIFY (trans1.scale() == 1.0);
    QVERIFY (trans1.orientation() == M::Landscape);

    SYS_DEBUG ("Checking the set functions...");
    trans2.setOffset (QPointF(10, 11));
    trans2.setScale (2.0);
    trans2.setOrientation (M::Portrait);
    trans2.setExpectedSize (QSize(864, 480));

    QVERIFY (trans2.x() == 10);
    QVERIFY (trans2.y() == 11);
    QVERIFY (trans2.scale() == 2.0);
    QVERIFY (trans2.orientation() == M::Portrait);
    QVERIFY (trans2.expectedWidth() == 864);
    QVERIFY (trans2.expectedHeight() == 480);

    SYS_DEBUG ("Testing the copy constructor.");
    WallpaperITrans trans3 (trans2);
    QVERIFY (trans3.x() == 10);
    QVERIFY (trans3.y() == 11);
    QVERIFY (trans3.scale() == 2.0);
    QVERIFY (trans3.orientation() == M::Portrait);
    QVERIFY (trans3.expectedWidth() == 864);
    QVERIFY (trans3.expectedHeight() == 480);

    SYS_DEBUG ("testing operator=...");
    trans1 = trans2;
    QVERIFY (trans1.x() == 10);
    QVERIFY (trans1.y() == 11);
    QVERIFY (trans1.scale() == 2.0);
    QVERIFY (trans1.orientation() == M::Portrait);
    QVERIFY (trans1.expectedWidth() == 864);
    QVERIFY (trans1.expectedHeight() == 480);

    SYS_DEBUG ("Testing operator*...");
    SYS_DEBUG ("*** trans1.scale() = %d", trans2 * 2);
    QVERIFY (trans2 * 2 == 4);
}

/*!
 * This function will try to find the first available wallpaper that is not the
 * current wallpaper and will set it with various image transformations. The
 * saved images will be loaded to test the availablity and the size.
 */
void
Ut_WallpaperBusinessLogic::testSetWallpapert ()
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
    // FIXME: This should not be needed...
    landscapeITrans.setExpectedSize (QSize(864, 480));
    portraitITrans.setExpectedSize (QSize(480, 864));
    m_Api->setBackground (
            &landscapeITrans,
            &portraitITrans,
             availableWallpapers[n]);

    testValidImages ();

    /*
     * Testing with some arbitrary scale and offset images.
     */
    landscapeITrans.setScale (0.3);
    portraitITrans.setScale (0.2);   
    landscapeITrans.setOffset (QPointF(10, 20));
    portraitITrans.setOffset (QPointF(20, 10));

    m_Api->setBackground (
            &landscapeITrans,
            &portraitITrans,
             availableWallpapers[n]);
    
    testValidImages ();
    
    /*
     * Also with magnifying and negative offsets.
     */
    landscapeITrans.setScale (1.5);
    portraitITrans.setScale (1.3);   
    landscapeITrans.setOffset (QPointF(-10, -200));
    portraitITrans.setOffset (QPointF(-800, -300));

    m_Api->setBackground (
            &landscapeITrans,
            &portraitITrans,
             availableWallpapers[n]);
    
    testValidImages ();
}

/******************************************************************************
 * Private functions.
 */

/*!
 * This low level function will check the GConf database, read the image file
 * names and see if it is possible to load them. If the images can be loaded the
 * function will test if the image sizes are correct.
 */
void
Ut_WallpaperBusinessLogic::testValidImages ()
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

QTEST_APPLESS_MAIN(Ut_WallpaperBusinessLogic)


