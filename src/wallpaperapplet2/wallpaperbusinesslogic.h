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

#include <QList>
#include <QObject>
#include <meegocontrolexport.h>

class QString;
class MGConfItem;
class WallpaperDescriptor;
class QStringList;
class WallpaperITrans;

#include <QPointer>
#include <QtConcurrentRun>
#include <QFutureWatcher>

#include <wallpaperdescriptor.h>

#define WALLPAPER_DBUS_INTERFACE "com.nokia.wallpaper"
#define WALLPAPER_DBUS_EDIT_SIGNAL "edit"


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

    void setEditedImage (WallpaperDescriptor *desc, bool ours = false);
    void startEdit (WallpaperDescriptor *desc = NULL);

    WallpaperDescriptor *editedImage ();

    void setBackground (
        WallpaperITrans     *landscapeITrans,
        WallpaperITrans     *portraitITrans,
        WallpaperDescriptor *desc = 0);

    signals:
        void wallpaperChanged ();
   
    void imageEditRequested ();
    
    private slots:
        bool portraitGConfChanged ();

    void editRequestArrived (
        QString   portraitFileName,
        QString   landscapeFileName);

    void startEditThreadEnded ();
    void valueChanged ();
    
private:
    QString dirPath (bool downloadDir = false) const;
    bool ensureHasDirectory ();
    void createBackupFiles ();
    void deleteBackupFiles ();
    void saveOriginal (
        WallpaperDescriptor *desc);
    bool writeFiles (
        WallpaperITrans     *landscapeITrans,
        WallpaperITrans     *portraitITrans,
        WallpaperDescriptor *desc);

    void makeBackup (const QString &filePath);
    void makeImageFile (
            const QString        &filePath,
            WallpaperDescriptor  *desc,
            WallpaperITrans      *transformations);

    bool supportsLandscape () const;
    bool supportsPortrait () const;

    private:
        QPointer<MGConfItem>       m_PPItem;
        QPointer<MGConfItem>       m_POItem;

    QPointer<WallpaperDescriptor>  m_EditedImage;
    bool                           m_EditedImageOurs;
    bool                           m_OrientationLocked;
    M::Orientation                 m_LockedOrientation;
    QFutureWatcher <void>          m_FutureWatcher;

#ifdef UNIT_TEST
    friend class Ut_WallpaperBusinessLogic;
    friend class Ft_WallpaperBusinessLogic;
#endif
};

#endif
