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
#ifndef WALLPAPERUTILS_H
#define WALLPAPERUTILS_H

#include <QString>
#include <QSet>

class QStringList;

namespace Wallpaper 
{
    QString constructPath (
            const QString &dirName,
            const QString &filename = QString());

    QString logicalIdToFilePath (
            const QString    &id);

    bool isInDir (
            const QString  &directory,
            const QString  &filePath);

    QStringList imageNameFilter ();

    QSet<QString> readDir (
            const QString     &directoryPath,
            const QStringList &nameFilters);

    bool imageFile (
            const QString     &filePath);
};
#endif
