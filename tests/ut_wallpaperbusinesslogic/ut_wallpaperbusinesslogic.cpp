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

#include "filesystemstub.h"
#include "ut_wallpaperbusinesslogic.h"
//#include "wallpapergconf.h"

#include "wallpaperbusinesslogic.h"
#include "wallpaperitrans.h"
//#include "wallpapergconf.h"
#include "wallpaperutils.h"
#include "wallpaperconfiguration.h"

#include <MApplication>
#include <MGConfItem>

#define DEBUG
#include "../../src/debug.h"

static const QString PortraitKey = 
    "/desktop/meego/background/portrait/picture_filename";
static const QString LandscapeKey = 
    "/desktop/meego/background/landscape/picture_filename";

/******************************************************************************
 * GConf stub.
 */
#include <gconf/gconf-client.h>
#include <gconf/gconf-value.h>

static QHash<QString, QString> MyGConfDatabase;

GConfValue *
gconf_client_get (
        GConfClient* client,
        const gchar* key,
        GError** err)
{
    Q_UNUSED (client);
    Q_UNUSED (err);
    Q_ASSERT (qstrlen (key) >= 10);

    QString myKey = key;
    GConfValue *retval;

    SYS_WARNING ("*** key = %s", SYS_STR(myKey));
    if (MyGConfDatabase.contains(myKey)) {
        const char *r = strdup(MyGConfDatabase[myKey].toUtf8().constData());

        retval = gconf_value_new (GCONF_VALUE_STRING);
        gconf_value_set_string (retval, r);
        SYS_DEBUG ("returning '%s'", r);
    } else {
        SYS_DEBUG ("returning false");
        retval = gconf_value_new (GCONF_VALUE_BOOL);
    }

    return retval;
}

/******************************************************************************
 * QImage stub.
 */
#include <QImage>

bool 	
QImage::load (
        const QString  &fileName, 
        const char     *format)
{
    Q_UNUSED (fileName);
    Q_UNUSED (format);
    SYS_DEBUG ("*** fileName = %s", SYS_STR(fileName));
    return true;
}

#include <QPixmap>
bool
QPixmap::load (
        const QString  &fileName, 
        const char     *format, 
        Qt::ImageConversionFlags flags)
{
    Q_UNUSED (fileName);
    Q_UNUSED (format);
    Q_UNUSED (flags);
    SYS_DEBUG ("*** fileName = %s", SYS_STR(fileName));
    return true;
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
    m_WallpaperChangedCame = false;
}

void
SignalSink::wallpaperChanged ()
{
    SYS_DEBUG ("");
    m_WallpaperChangedCame = true;
}

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
static char* app_name = (char*) "./ut_wallpaperbusinesslogic";

void 
Ut_WallpaperBusinessLogic::initTestCase()
{
    bool connectSuccess;

    MyGConfDatabase["/meegotouch/theme/name"] = "MyThemeName";
    MyGConfDatabase[Wallpaper::CurrentPortraitKey] = 
        "/ak/current.jpg";
    MyGConfDatabase[Wallpaper::OriginalPortraitKey] = "";

    m_App = new MApplication (argc, &app_name);


    m_Api = new WallpaperBusinessLogic;
    connectSuccess = connect (
            m_Api, SIGNAL (wallpaperChanged()),
            &m_SignalSink, SLOT (wallpaperChanged()));
    QVERIFY (connectSuccess);


    //QVERIFY (m_Api->m_EditedImage == 0);
}

void 
Ut_WallpaperBusinessLogic::cleanupTestCase()
{
    delete m_Api;

    m_App->deleteLater ();
}

/*!
 * Checks if the GConfItem objects are created with the right keys.
 */
void
Ut_WallpaperBusinessLogic::testGConfItems ()
{
    QVERIFY (m_Api->m_PPItem != NULL);
    QVERIFY (m_Api->m_PPItem->key() == Wallpaper::CurrentPortraitKey);

    QVERIFY (m_Api->m_POItem != NULL);
    QVERIFY (m_Api->m_POItem->key() == Wallpaper::OriginalPortraitKey);

    QVERIFY (m_Api->m_PHItem != NULL);
    QVERIFY (m_Api->m_PHItem->key() == Wallpaper::PortraitHistoryKey);
}

/*!
 */
void
Ut_WallpaperBusinessLogic::testCurrentWallpaper ()
{
    QString currentFilePath;
    QString originalFilePath;


    m_Api->currentWallpaper (currentFilePath, originalFilePath);
    QVERIFY (currentFilePath == "/ak/current.jpg");
    QVERIFY (originalFilePath.isEmpty());

    SYS_DEBUG ("*** currentFilePath  = %s", SYS_STR(currentFilePath));
    SYS_DEBUG ("*** originalFilePath = %s", SYS_STR(originalFilePath));
}

void
Ut_WallpaperBusinessLogic::testEditedImage ()
{
#if 0
    WallpaperDescriptor desc;

    m_Api->setEditedImage (&desc);
    QVERIFY (m_Api->editedImage() == &desc);

    m_Api->setEditedImage (0);
    QVERIFY (m_Api->editedImage() == 0);
#endif
}

/*
 * The tracker is stubbed, so we can test the available wllpapers. It is only
 * the current wallpaper singleton, we have to do something with that too...
 */
void
Ut_WallpaperBusinessLogic::testAvailableWallpapers ()
{
#if 0
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
        QString   filename, basename;
        QString   extension, mimetype;
        QString   suggestedp, suggestedl;
        QString   originalp, originall;
        QString   imageID;

        filename = desc->filename ();
        imageID = desc->imageID ();
        basename = desc->basename ();
        extension = desc->extension ();
        mimetype = desc->mimeType ();
        suggestedp = desc->suggestedOutputFilename (M::Portrait);
        suggestedl = desc->suggestedOutputFilename (M::Landscape);
        originalp = desc->originalImageFile (M::Portrait);
        originall = desc->originalImageFile (M::Landscape);

        if ((filename.isEmpty() && imageID.isEmpty()) ||
                (basename.isEmpty() && imageID.isEmpty()) ||
                (mimetype.isEmpty() && imageID.isEmpty()) ||
                suggestedp.isEmpty() ||
                suggestedl.isEmpty() ||
                (originalp.isEmpty() && imageID.isEmpty()) ||
                (originall.isEmpty() && imageID.isEmpty())) {
            /*
             * These might prove usefull in the future, but obviously generate
             * too much output.
             */
            SYS_DEBUG ("*** available wallpaper #%3d ***", n);
            SYS_DEBUG ("*** filename   = %s", SYS_STR(filename));
            SYS_DEBUG ("*** imageID    = %s", SYS_STR(imageID));
            SYS_DEBUG ("*** basename   = %s", SYS_STR(basename));
            SYS_DEBUG ("*** mimetype   = %s", SYS_STR(mimetype));
            SYS_DEBUG ("*** extension  = %s", SYS_STR(extension));
            SYS_DEBUG ("*** suggestedp = %s", SYS_STR(suggestedp));
            SYS_DEBUG ("*** suggestedl = %s", SYS_STR(suggestedl));
            SYS_DEBUG ("*** originalp  = %s", SYS_STR(originalp));
            SYS_DEBUG ("*** originall  = %s", SYS_STR(originall));
        }

//        QVERIFY (!filename.isEmpty() || !imageID.isEmpty());
//        QVERIFY (!basename.isEmpty() || !imageID.isEmpty());
//        QVERIFY (!mimetype.isEmpty() || !imageID.isEmpty());
//        QVERIFY (!suggestedp.isEmpty());
//        QVERIFY (!suggestedl.isEmpty());
//        QVERIFY (!originalp.isEmpty() || !imageID.isEmpty());
//        QVERIFY (!originall.isEmpty() || !imageID.isEmpty());

        ++n;
    }
#endif
}


/*!
 * Checks the low level WallpaperITrans class, its tag methods and overloaded
 * operators.
 */
void
Ut_WallpaperBusinessLogic::testITrans ()
{
#if 0
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
#endif
}

void
Ut_WallpaperBusinessLogic::testCreateDirectory ()
{
#if 0
    m_Api->ensureHasDirectory ();
    m_Api->ensureHasDirectory ();

    QString dirPath = m_Api->dirPath ();
    QDirStub dir (dirPath);
    QVERIFY (dir.exists());
#endif
}

void
Ut_WallpaperBusinessLogic::testBackupFiles ()
{
#if 0
    QFileStub desktopFile(m_Api->dirPath() + "wallpaper.desktop");
    desktopFile.open (QIODevice::WriteOnly);

    QFileStub desktopFileBak (m_Api->dirPath() + "wallpaper.desktop.BAK");

    /*
     *
     */
    m_Api->createBackupFiles ();

    SYS_DEBUG ("*** desktopfile.exists = %s",
            SYS_BOOL(desktopFile.exists()));
    SYS_DEBUG ("*** backupfile.exists  = %s",
            SYS_BOOL(desktopFileBak.exists()));
    QVERIFY (!desktopFile.exists());
    QVERIFY (desktopFileBak.exists());

    m_Api->deleteBackupFiles ();
    QVERIFY (!desktopFileBak.exists());
#endif
}

#if 0
/*
 * This test was remoed because it is sometimes fails on cita with a segfault. I
 * think the QIODevice::writeData() should be stubbed, because it is somehow
 * changed. It did not cause any problem before...
 */
void
Ut_WallpaperBusinessLogic::testSetBackground()
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

    m_SignalSink.reset ();
    SYS_WARNING ("======================================================");
    m_Api->setBackground (
            &landscapeITrans,
            &portraitITrans,
             availableWallpapers[n]);
    // Testing if the images are valid and we got a signal about the change.
    QVERIFY (m_SignalSink.m_WallpaperChangedCame);
}
#endif

#if 0
/*
 * I suspect this check is not right, also it is a feature that we might remove.
 */
void
Ut_WallpaperBusinessLogic::testCheckForPendingSignals()
{
    MGConfItem requestCodeItem  (WALLPAPER_APPLET_REQUEST_CODE);
    requestCodeItem.set(WallpaperRequestEdit);
    m_Api->checkForPendingSignals();

    QVERIFY (m_SignalSink.m_WallpaperImageEditRequestedCame);
}
#endif


/******************************************************************************
 * Private functions.
 */
QTEST_APPLESS_MAIN(Ut_WallpaperBusinessLogic)


