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
** Contact: Karoliina T. Salminen <karoliina.t.salminen@nokia.com>
** Authors: David Kedves <dkedves@blumsoft.eu>
**          Laszlo Pere <lpere@blumsoft.eu>
**
****************************************************************************/
#include "wallpaperbusinesslogic.h"
#include "wallpaperutils.h"
#include "wallpaperconfiguration.h"

#include "wallpapergconf.h"
#include "wallpaperdescriptor.h"
#include "wallpaperitrans.h"
#include "wallpapereditorsheet.h"


#include <QString>
#include <QStringList>
#include <QProcessEnvironment>
#include <QPainter>
#include <QDBusConnection>
#include <QDBusError>
#include <QFile>
#include <MTheme>
#include <MGConfItem>
#include <MApplication>
#include <MApplicationWindow>

#define DEBUG
#define WARNING
#include "../debug.h"

static const QString wallpaperDir = ".wallpapers";
static const QString wallpapersDir = "MyDocs/.wallpapers";
static const QString destopFileName = "wallpaper.desktop";
static const QString backupExtension = ".BAK";
static const QString saveFileExtension = ".png";
static const QString saveFileMimeType = "image/png";
static const QString nl = "\n";

WallpaperBusinessLogic::WallpaperBusinessLogic() :
    m_OrientationLocked (false)
{
    m_PPItem = new MGConfItem (Wallpaper::CurrentPortraitKey, this);
    m_POItem = new MGConfItem (Wallpaper::OriginalPortraitKey, this);

    connect (m_PPItem, SIGNAL(valueChanged ()),
            this, SLOT(portraitGConfChanged()));

    connect (m_POItem, SIGNAL(valueChanged ()),
            this, SLOT(portraitGConfChanged()));
}

WallpaperBusinessLogic::~WallpaperBusinessLogic()
{
}

/*!
 * \param landscapeITrans Image transformations for the landscape orientation.
 * \param portraitITrans Image transformations for the portrait orientation.
 * \param desc The image descriptor that holds the image(s).
 *
 * A high level method to set the current wallpaper. This method will load the
 * image file(s), apply the image transformations to create the portrait and
 * landscape variants then will save the modified images and set them as
 * wallpaper image(s). This method will also save some metadata that makes it
 * possible to re-edit the images using the image transformations as default
 * values for the image editor widget.
 */
void
WallpaperBusinessLogic::setBackground (
        WallpaperITrans     *landscapeITrans,
        WallpaperITrans     *portraitITrans,
        WallpaperDescriptor *desc)
{
#if 0
    bool success;

    if (desc == 0)
        desc = m_EditedImage;

    Q_ASSERT (landscapeITrans);
    Q_ASSERT (portraitITrans);
    Q_ASSERT (desc);

    success = ensureHasDirectory ();
    if (!success)
        return;

    createBackupFiles ();

    success = writeFiles (landscapeITrans, portraitITrans, desc);
    if (!success) {
        // FIXME: Should restore backup files here.
        return;
    }
   
    deleteBackupFiles ();

    /*
     * Re-reading the current wallpaper as it is saved into the user's home.
     */
    WallpaperCurrentDescriptor *currentDesc;
    currentDesc = WallpaperCurrentDescriptor::instance ();
    currentDesc->setFromDesktopFile (dirPath() + destopFileName);
    emit wallpaperChanged ();
#endif
}


void
WallpaperBusinessLogic::startEdit (
        WallpaperDescriptor desc)
{
    SYS_DEBUG ("");
    if (!m_EditedImage.isNull()) {
        SYS_DEBUG ("We already have an edited image, ignoring this request.");
        return;
    }

    m_EditedImage = desc;
    emit editWallpaper (desc);
}

void
WallpaperBusinessLogic::endEdit ()
{
    m_EditedImage = WallpaperDescriptor ();
}

bool
WallpaperBusinessLogic::setWallpaper ()
{
    QString  filePath = m_EditedImage.filePath();
    QString  basename = Wallpaper::baseName (filePath);
    QFile    origFile (filePath);
    QString  targetPath;
    bool     success;

    /*
     * Checking if our output directory exists, creating if not.
     */
    targetPath = Wallpaper::constructPath (Wallpaper::ImagesSaveDir);
    success = Wallpaper::ensureHasDirectory (targetPath);
    if (!success)
        goto finalize;

    /*
     *
     */
    targetPath = Wallpaper::constructPath (targetPath, basename);
    SYS_DEBUG ("*** filePath   = %s", SYS_STR(filePath));
    SYS_DEBUG ("*** basename   = %s", SYS_STR(basename));
    SYS_DEBUG ("*** targetPath = %s", SYS_STR(targetPath));
    
    if (filePath == targetPath) {
        success = true;
        goto finalize;
    }

    success = origFile.copy(targetPath);
    if (success) {
        goto finalize;
    } else {
        SYS_WARNING ("WARNING: can't copy file to %s", SYS_STR(targetPath));
        QFile targetFile (targetPath);
        if (targetFile.exists()) {
            SYS_WARNING ("WARNING: File exists, trying to overwrite.");
            success = targetFile.remove ();

            if (!success) {
                SYS_WARNING ("ERROR: removing '%s': %m", SYS_STR(targetPath));
                goto finalize;
            }
        }
    }
    
    success = origFile.copy(targetPath);
    if (!success) {
        SYS_WARNING ("ERROR copying file: %m");
    }

finalize:
    if (success) {
        m_POItem->set (filePath);
        m_PPItem->set (targetPath);
    }

    return success;
}

bool
WallpaperBusinessLogic::setWallpaper (
        QPixmap   &pixmap)
{
    QString  filePath = m_EditedImage.filePath();
    QString  basename = Wallpaper::baseName (filePath);
    QFile    targetFile;
    QString  targetPath;
    bool     success;

    /*
     * Checking if our output directory exists, creating if not.
     */
    targetPath = Wallpaper::constructPath (Wallpaper::ImagesSaveDir);
    success = Wallpaper::ensureHasDirectory (targetPath);
    if (!success)
        goto finalize;

    basename = Wallpaper::setFileExtension (basename, Wallpaper::OutImgExt);
    targetPath = Wallpaper::constructPath (targetPath, basename);
    targetFile.setFileName (targetPath);
    if (targetFile.exists()) {
        success = targetFile.remove ();
        if (!success) {
                SYS_WARNING ("ERROR: removing '%s': %m", SYS_STR(targetPath));
                goto finalize;
        }
    }

    success = pixmap.save (targetPath);
    if (!success) {
        SYS_WARNING ("ERROR: Saving file to %s failed: %m", SYS_STR(targetPath));
    }

finalize:
    if (success) {
        SYS_DEBUG ("Saved to '%s'", SYS_STR(targetPath));
        m_POItem->set (filePath);
        m_PPItem->set (targetPath);
    }

    return success;
}


/*!
 * Returns the wallpaper that is being edited.
 */
WallpaperDescriptor 
WallpaperBusinessLogic::editedImage () const
{
    return m_EditedImage;
}

/*********************************************************************************
 * Low level file handling functions.
 */
bool
WallpaperBusinessLogic::supportsLandscape () const
{
    return !m_OrientationLocked || m_LockedOrientation == M::Landscape;
}

bool
WallpaperBusinessLogic::supportsPortrait () const
{
    return !m_OrientationLocked || m_LockedOrientation == M::Portrait;
}

/******************************************************************************
 * Handling the GConf database.
 */
void
WallpaperBusinessLogic::portraitGConfChanged ()
{
    SYS_DEBUG ("");
    emit wallpaperChanged ();
}

void
WallpaperBusinessLogic::currentWallpaper (
        QString   &currentFilePath,
        QString   &originalFilePath)
{
    currentFilePath  = m_PPItem->value().toString ();
    originalFilePath = m_POItem->value().toString ();

    currentFilePath  = Wallpaper::logicalIdToFilePath (currentFilePath);
    originalFilePath = Wallpaper::logicalIdToFilePath (originalFilePath);
}
