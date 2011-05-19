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

#include "wallpaperdescriptor.h"
#include "wallpaperdescriptor_p.h"

#include <QUrl>
#include <QDir>

#define DEBUG
#define WARNING
#include "../debug.h"


/******************************************************************************
 * WallpaperDescriptor implementation: constructors and destructors.
 */
WallpaperDescriptor::WallpaperDescriptor(
        QObject         *parent) : 
    QObject (parent),
    m_Priv (new WallpaperDescriptorPrivate)
{
}

WallpaperDescriptor::WallpaperDescriptor(
        const QString  &filePath,
        QObject        *parent) : 
    QObject (parent),
    m_Priv (new WallpaperDescriptorPrivate (filePath))
{
}


WallpaperDescriptor::WallpaperDescriptor (
        const WallpaperDescriptor &orig) :
    QObject ()
{
    m_Priv = orig.m_Priv;

    if (m_Priv) 
        m_Priv->ref ();    
}

WallpaperDescriptor::~WallpaperDescriptor ()
{
    if (m_Priv) {
        if (m_Priv->unRef() == 0)
            delete m_Priv;
    }
}

WallpaperDescriptor &
WallpaperDescriptor::operator= (
         const WallpaperDescriptor &rhs)
{
    if (this == &rhs)
        return *this;

    if (m_Priv) {
        if (m_Priv->unRef() == 0)
            delete m_Priv;
    }

    m_Priv = rhs.m_Priv;
    if (m_Priv)
        m_Priv->ref ();

    return *this;
}

/******************************************************************************
 * Standard set/get methods.
 */
void
WallpaperDescriptor::setFilePath (
        const QString &filePath)
{
    m_Priv->setFilePath (filePath);
}

QString
WallpaperDescriptor::filePath () const
{
    return m_Priv->filePath ();
}

QUrl
WallpaperDescriptor::url () const
{
    QUrl    retval;
    QString path;

    path = m_Priv->filePath ();
    if (path.startsWith(QDir::separator())) {
        path.prepend ("file://");
        retval.setEncodedUrl (path.toAscii());
    }

    return retval;
}

static const char *
mimetypes[][2] = 
{
    { ".bmp",  "image/bmp"  },
    { ".gif",  "image/gif"  },
    { ".jpg",  "image/jpeg" },
    { ".jpeg", "image/jpeg" },
    { ".jpe",  "image/jpeg" },
    { ".tif",  "image/tiff" },
    { ".png",  "image/png"  },
    { NULL,   NULL}
};

QString
WallpaperDescriptor::mimeType () const
{
    QString retval;
    QString path;

    path = m_Priv->filePath();
    if (path.isEmpty()) 
        goto finalize;

    for (int n = 0; mimetypes[n][0] != NULL; ++n) {
        if (path.endsWith(mimetypes[n][0], Qt::CaseInsensitive)) {
            retval = QString(mimetypes[n][1]);
            break;
        }
    }

finalize:
    return retval;
}

void
WallpaperDescriptor::setThumbnailPending (
        bool pending)
{
    m_Priv->setThumbnailPending (pending);
}

bool
WallpaperDescriptor::thumbnailPending () const
{
    return m_Priv->thumbnailPending ();
}
     
void 
WallpaperDescriptor::setThumbnail (
        const QPixmap &thumbnail)
{
    m_Priv->setThumbnail (thumbnail);
}

void
WallpaperDescriptor::unsetThumbnail ()
{
    m_Priv->unsetThumbnail ();
}

QPixmap
WallpaperDescriptor::thumbnail () const
{
    return m_Priv->thumbnail ();
}

bool
WallpaperDescriptor::hasThumbnail () const
{
    return m_Priv->hasThumbnail ();
}


