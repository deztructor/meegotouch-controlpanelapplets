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
#include "wallpaperdescriptor_p.h"
#include "wallpaperutils.h"
#define DEBUG
#define WARNING
#include "../debug.h"

WallpaperDescriptorPrivate::WallpaperDescriptorPrivate () :
    m_ReferenceCounter (1),
    m_Selected (false),
    m_Progress (false),
    m_ThumbnailPending (false),
    m_HasThumbnail (false),
    m_HistoryIndex (-1)
{
}

WallpaperDescriptorPrivate::WallpaperDescriptorPrivate (
        const QString &filePath) :
    m_ReferenceCounter (1),
    m_FilePath (filePath),
    m_Selected (false),
    m_Progress (false),
    m_ThumbnailPending (false),
    m_HasThumbnail (false),
    m_HistoryIndex (-1)
{
}


void
WallpaperDescriptorPrivate::ref ()
{
    ++m_ReferenceCounter;
}

int 
WallpaperDescriptorPrivate::unRef ()
{
    return --m_ReferenceCounter;
}

/******************************************************************************
 * Standard set/get methods.
 */
void 
WallpaperDescriptorPrivate::setFilePath (
        const QString &filePath)
{
    m_FilePath = filePath;
}
        
QString 
WallpaperDescriptorPrivate::filePath () const
{
    return m_FilePath;
}

void
WallpaperDescriptorPrivate::setThumbnailPending (
        bool pending)
{
    m_ThumbnailPending = pending;
}

bool
WallpaperDescriptorPrivate::thumbnailPending () const
{
    return m_ThumbnailPending;
}

void 
WallpaperDescriptorPrivate::setThumbnail (
        const QPixmap &thumbnail)
{
    m_HasThumbnail = true;
    m_Thumbnail = thumbnail;
}

void
WallpaperDescriptorPrivate::unsetThumbnail ()
{
    m_HasThumbnail = false;
    m_ThumbnailPending = false;
    m_Thumbnail = QPixmap();
}

QPixmap
WallpaperDescriptorPrivate::thumbnail () const
{
    return m_Thumbnail;
}

bool
WallpaperDescriptorPrivate::hasThumbnail () const
{
    return m_HasThumbnail;
}

void
WallpaperDescriptorPrivate::setSelected (
        bool selected)
{
    m_Selected = selected;
}

bool 
WallpaperDescriptorPrivate::selected () const
{
    return m_Selected;
}

void
WallpaperDescriptorPrivate::setProgress (
        bool progress)
{
    m_Progress = progress;
}

bool 
WallpaperDescriptorPrivate::progress () const
{
    return m_Progress;
}

void
WallpaperDescriptorPrivate::setHistoryIndex (
        int   index)
{
    m_HistoryIndex = index;
}

int 
WallpaperDescriptorPrivate::historyIndex () const
{
    return m_HistoryIndex;
}

/******************************************************************************
 * Loading...
 */
QuillImage
WallpaperDescriptorPrivate::load (
        const QSize    &expectedSize,
        QSize          &originalSize)
{
    QuillImage        retval;
    QSize             mySize;
    QuillImageFilter *loadFilter;

    QuillFile quillFile  (filePath());
    mySize = quillFile.fullImageSize ();
    originalSize = mySize;
   
    SYS_DEBUG ("*** expectedSize = %s", SYS_SIZE(expectedSize));
    SYS_DEBUG ("*** mySize       = %s", SYS_SIZE(mySize));

    if (Wallpaper::smallerSize(mySize, expectedSize))
        mySize.scale (expectedSize, Qt::KeepAspectRatio);
    else
        mySize.scale (expectedSize, Qt::KeepAspectRatioByExpanding);

    loadFilter = QuillImageFilterFactory::createImageFilter(
            QuillImageFilter::Role_Load);
    loadFilter->setOption(
            QuillImageFilter::FileName,
            QVariant(filePath()));
    retval = loadFilter->apply (QImage(mySize, QImage::Format_RGB16));
    delete loadFilter;

    return retval;
}

