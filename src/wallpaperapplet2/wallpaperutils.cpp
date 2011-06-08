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
#include "wallpaperutils.h"
#include "wallpaperconfiguration.h"
#include "wallpaperitrans.h"

using namespace Wallpaper;

#include <stdlib.h>
#include <QSize>
#include <QChar>
#include <QString>
#include <QStringList>

#include <QDir>
#include <QFile>
#include <QFileInfoList>
#include <MGConfItem>
//#include <QImage>
//#include <QPainter>

#undef DEBUG
#define WARNING
#include <../debug.h>


static const QChar Tilde('~');
static const QChar ExtSep('.');

QString 
Wallpaper::constructPath (
        const QString &dirName,
        const QString &filename)
{
    QString retval = dirName;

    if (retval.contains(Tilde)) {
        QString homeDir (getenv("HOME"));

        if (homeDir.isEmpty())
            homeDir = "/home/user";

        retval.replace (Tilde, homeDir);
    }

    if (!filename.isEmpty()) {
        if (!retval.endsWith (QDir::separator())) 
            retval += QDir::separator();

        retval += filename;
    }

    SYS_WARNING ("returning %s", SYS_STR(retval));
    return retval;
}

QString 
Wallpaper::baseName (
        const QString &filePath)
{
    QString retval = filePath;
    int     position = retval.lastIndexOf (QDir::separator());

    if (position >= 0)
        retval.remove (0, position + 1);

    return retval;
}

QString 
Wallpaper::setFileExtension (
        const QString &fileName,
        const QString &extension)
{
    QString myExtension = extension;
    QString retval = fileName;

    if (!myExtension.startsWith(ExtSep))
        myExtension.prepend(ExtSep);

    if (!fileName.endsWith(myExtension)) {
        int index = retval.lastIndexOf (ExtSep);

        if (index >= 0)
            retval.truncate (index);

        retval += extension;
    }

    return retval;
}

QString 
Wallpaper::logicalIdToFilePath (
        const QString    &id)
{
    QString retval = id;

    if (!id.isEmpty() && !id.startsWith (QDir::separator())) {
        MGConfItem themeItem (Wallpaper::themeNameKey);
        QString    themeName = themeItem.value().toString();

        retval = Wallpaper::themeImagePath.arg(themeName).arg(id);
    }

    SYS_DEBUG ("Returning '%s'", SYS_STR(retval));
    return retval;
}

bool
Wallpaper::isInDir (
        const QString  &directory,
        const QString  &filePath)
{
    return filePath.startsWith(directory);
}

QStringList
Wallpaper::imageNameFilter ()
{
    QStringList retval;

    retval << "*.jpg" << "*.jpeg" << "*.jpe" <<  "*.png" << "*.bmp" << 
        "*.gif" << "*.tif";

    return retval;
}

/******************************************************************************
 * File-system manipulation methods.
 */
QHash<QString, qint64> 
Wallpaper::readDir (
        const QString     &directoryPath,
        const QStringList &nameFilters)
{
    QDir                   directory (directoryPath);
    QHash<QString, qint64> retval;
    QFileInfoList          entryList;

    if (!directory.exists(directoryPath))
        goto finalize;

    entryList = directory.entryInfoList (
            nameFilters, QDir::Files | QDir::NoSymLinks | QDir::Readable);

    for (int iList = 0; iList < entryList.count(); iList++) {
        SYS_DEBUG ("entryList[%d] = '%s'", iList, 
                SYS_STR(entryList[iList].filePath()));
        retval[entryList[iList].filePath()] = entryList[iList].size();
    }
    
finalize:
    return retval;
}

bool
Wallpaper::imageFile (
            const QString     &filePath)
{
    bool retval = false;

    if (filePath.startsWith(QDir::separator())) {
        QFile   thisFile(filePath);

        if (thisFile.exists())
            retval = true;
    }

    return retval;
}

bool
Wallpaper::ensureHasDirectory (
        const QString &directoryPath)
{
    QDir dir (directoryPath);
    bool retval;

    if (dir.exists()) {
        SYS_DEBUG ("Directory %s already exists.", SYS_STR(directoryPath));
        retval = true;
    } else if (!dir.mkpath(directoryPath)) {
        SYS_WARNING ("Unable to create %s directory.", SYS_STR(directoryPath));
    } else {
        retval = true;
    }

    return retval;
}

/******************************************************************************
 * Graphics.
 */
bool
Wallpaper::smallerSize (
        const QSize &a,
        const QSize &b)
{
    return a.width() < b.width() || a.height() < b.height();
}

#if 0
QImage
Wallpaper::rotate (
        QImage                  &image,
        const WallpaperITrans   &trans)
{
    QImage  retval;
    QSize   targetSize;
    QRectF  area;

    if (trans.rotation() == 90 || trans.rotation() == -90) {
        targetSize = QSize (
                image.height() * trans.scale(),
                image.width() * trans.scale());
    } else {
        targetSize = QSize (
                image.height() * trans.scale(),
                image.width() * trans.scale());
    }

    retval = QPixmap (targetSize);
    area = QRectF (0.0, 0.0, targetSize.width(), targetSize.height());
    QPainter  painter (&retval);

    painter.rotate (trans.rotation());
    painter.drawImage (area, m_Image);

    return retval;
}
#endif
