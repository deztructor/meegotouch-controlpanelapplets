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
using namespace Wallpaper;

#include <stdlib.h>
#include <QChar>
#include <QString>
#include <QStringList>
#include <QDir>

#define DEBUG
#define WARNING
#include <../debug.h>


static const QChar Tilde('~');

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

    return retval;
}

QStringList
Wallpaper::imageNameFilter ()
{
    QStringList retval;

    retval << "*.jpg" << "*.jpeg" << "*.jpe" <<  "*.png" << "*.bmp" << 
        "*.gif" << "*.tif";

    return retval;
}


QStringList
Wallpaper::readDir (
        const QString     &directoryPath,
        const QStringList &nameFilters)
{
    QDir          directory (directoryPath);
    QStringList   retval;
    QStringList   entryList;

    if (!directory.exists(directoryPath))
        goto finalize;

    entryList = directory.entryList (
            nameFilters, QDir::Files | QDir::NoSymLinks | QDir::Readable);

    for (int iList = 0; iList < entryList.count(); iList++) {
        retval << constructPath (directoryPath, entryList[iList]);
    }
    
finalize:
    SYS_DEBUG ("*** retval     = %s", SYS_STR(retval.join(";")));
    return retval;
}
