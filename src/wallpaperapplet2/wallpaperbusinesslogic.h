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
#ifndef WALLPAPERBUSINESSLOGIC_H
#define WALLPAPERBUSINESSLOGIC_H

#include <QObject>
//#include <meegocontrolexport.h>

class QString;
class MGConfItem;
class WallpaperDescriptor;
class QStringList;
class WallpaperITrans;
class WallpaperWorkerThread;

#include <QPointer>
#include <QuillImage>
#include <QSize>

#include <wallpaperdescriptor.h>

/*!
 * The logic which handling the loading/saving and management
 * work for wallpaper settings applet
 */
class MC_EXPORT WallpaperBusinessLogic : public QObject
{
    Q_OBJECT

    public:
        WallpaperBusinessLogic ();
        ~WallpaperBusinessLogic ();

        void currentWallpaper (
                QString   &currentFilePath,
                QString   &originalFilePath);

        void startEdit (WallpaperDescriptor desc);
        void endEdit ();
        bool setWallpaper ();
        bool setWallpaper (QPixmap &pixmap);
        bool loadImage (WallpaperDescriptor &desc);
        
        WallpaperDescriptor editedImage () const;
        QSize sceneSize () const;
        
    signals:
        void wallpaperChanged ();
        void wallpaperSaved ();
        void workerStarted ();
        void workerEnded ();
        void wallpaperLoaded (QuillImage image, QSize originalSize);
        void editWallpaper (WallpaperDescriptor desc);
   
    private slots:
        void portraitGConfChanged ();
        void workerThreadFinishedLoad ();
        void workerThreadFinishedSave ();

        #if 0
        bool supportsLandscape () const;
        bool supportsPortrait () const;
        #endif

    private:
        bool                       m_EditRequested;
        QPointer<MGConfItem>       m_PPItem;
        QPointer<MGConfItem>       m_POItem;
        WallpaperDescriptor        m_EditedImage;
        WallpaperWorkerThread     *m_WorkerThread;

#ifdef UNIT_TEST
    friend class Ut_WallpaperBusinessLogic;
    friend class Ft_WallpaperBusinessLogic;
#endif
};

#endif
