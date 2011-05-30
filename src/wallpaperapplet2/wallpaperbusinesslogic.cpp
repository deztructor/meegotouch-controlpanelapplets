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

WallpaperBusinessLogic::WallpaperBusinessLogic() 
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
