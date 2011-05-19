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
#ifndef WALLPAPERDESCRIPTOR_P_H
#define WALLPAPERDESCRIPTOR_P_H

#include <QPixmap>
#include <QString>
#include <QuillFile>
#include <QuillImage>
#include <QuillImageFilterFactory>
#include <QImage>

class WallpaperDescriptorPrivate
{
    public:
        WallpaperDescriptorPrivate ();
        WallpaperDescriptorPrivate (const QString &filePath);

        void ref ();
        int unRef ();

        /*
         * Standard set/get methods.
         */
        void setFilePath (const QString &filePath);
        QString filePath () const;

        void setThumbnailPending (bool pending);
        bool thumbnailPending () const;

        void setThumbnail (const QPixmap &thumbnail);
        void unsetThumbnail ();
        QPixmap thumbnail () const;
        bool hasThumbnail () const;

    private:
        int      m_ReferenceCounter;
        QString  m_FilePath;
        bool     m_ThumbnailPending;
        bool     m_HasThumbnail;
        QPixmap  m_Thumbnail;
};

#endif

