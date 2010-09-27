/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

/*
             Initiating the wallpaper editor screen from DBus

 In order to initiate the wallpaper editor from an external application two
 steps has to be performed.

 (1) First the controlpanel has to be started and the wallpaper applet must be
 activated through dbus. Please check the documentation of the
 libduicontrolpanel about this step.

 (2) Then the wallpaper applet can be contacted through DBus and the editor has
 to be initiated giving two image files as parameter. The first image file is
 the portrait, while the second is the landscape version of the wallpaper that
 should be editoed. The two images might be the same, in this case the very same
 image will be cropped differently for getting the portrait and the landscape
 variants. The following interface can be used for this step:

 dbus-send --session --type=signal / com.nokia.wallpaper.edit \
    string:/usr/share/themes/base/meegotouch/images/meegotouch-wallpaper-portrait.jpg \
    string:/usr/share/themes/base/meegotouch/images/meegotouch-wallpaper-landscape.jpg
*/ 
class WallpaperBusinessLogic : public QObject
{
    Q_OBJECT

public:
    typedef enum {
        FieldUrl  = 0,
        FieldTitle,
        FieldMime,
    } QueryFields;

    WallpaperBusinessLogic ();
    ~WallpaperBusinessLogic ();

    QList<WallpaperDescriptor *> availableWallpapers () const;

    void setEditedImage (WallpaperDescriptor *desc, bool ours = false);
    void startEdit (WallpaperDescriptor *desc = NULL);

    WallpaperDescriptor *editedImage ();

    void setBackground (
        WallpaperITrans     *landscapeITrans,
        WallpaperITrans     *portraitITrans,
        WallpaperDescriptor *desc = 0);

    void addImageFromGallery(QString);

signals:
    void wallpaperChanged ();
    void imageEditRequested ();
    
private slots:
    void editRequestArrived (
        QString   portraitFileName,
        QString   landscapeFileName);

    void startEditThreadEnded ();
    void valueChanged ();
    
private:
    QString dirPath () const;
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
    
private:
    MGConfItem   *m_LandscapeGConfItem;
    MGConfItem   *m_PortraitGConfItem;

    QPointer<WallpaperDescriptor> m_EditedImage;
    bool                          m_EditedImageOurs;
    QFutureWatcher <void>         m_FutureWatcher;

#ifdef UNIT_TEST
    friend class Ut_WallpaperBusinessLogic;
    friend class Ft_WallpaperBusinessLogic;
#endif
};

#endif
