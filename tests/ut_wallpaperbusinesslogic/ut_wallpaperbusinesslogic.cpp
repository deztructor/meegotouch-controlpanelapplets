/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#include "ut_wallpaperbusinesslogic.h"

#include "wallpaperbusinesslogic.h"
#include "wallpapercurrentdescriptor.h"

#include <MApplication>

#define DEBUG
#include "../../src/debug.h"


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

QTEST_APPLESS_MAIN(Ut_WallpaperBusinessLogic)


