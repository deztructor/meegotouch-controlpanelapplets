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
#include "wallpaperbusinesslogic.h"
#include "wallpaperdescriptor.h"
    
WallpaperBusinessLogic::WallpaperBusinessLogic (
        QObject *parent) :
    QObject (parent)
{
    Q_UNUSED (parent);
}

WallpaperBusinessLogic::~WallpaperBusinessLogic ()
{

}

QList<WallpaperDescriptor *>
WallpaperBusinessLogic::availableWallpapers () const
{
    QList<WallpaperDescriptor *> testList;
    WallpaperDescriptor *desc;

    desc = new WallpaperDescriptor;

    if (supportsLandscape()) {
        desc->setImageID ("meegotouch-wallpaper-landscape", 
                WallpaperDescriptor::Landscape);
        desc->setImageID ("meegotouch-wallpaper-landscape", 
                WallpaperDescriptor::OriginalLandscape);
    }

    if (supportsPortrait()) {
        desc->setImageID ("meegotouch-wallpaper-portrait",
                WallpaperDescriptor::Portrait);
        desc->setImageID ("meegotouch-wallpaper-portrait",
                WallpaperDescriptor::OriginalPortrait);
    }

    testList << desc;

    desc = new WallpaperDescriptor;

    desc->setUrl ("file:///NoDir/NoSuchFile.png");
    desc->setMimeType ("image/png");

    testList << desc;

    desc = new WallpaperDescriptor;

    desc->setUrl ("file:///AnotherSimulatedDir/WithASimulatedFile.jpg");
    desc->setMimeType ("image/jpg");

    testList << desc;

    return testList;
}

void 
WallpaperBusinessLogic::directoryChangedDelayed ()
{

}

void
WallpaperBusinessLogic::setEditedImage (
    WallpaperDescriptor *desc,
    bool ours)
{
    m_EditedImage = desc;
    m_EditedImageOurs = ours;
}

void
WallpaperBusinessLogic::startEdit (WallpaperDescriptor *desc)
{
    Q_UNUSED (desc);

    emit imageEditRequested ();
}

WallpaperDescriptor *
WallpaperBusinessLogic::editedImage ()
{
    return m_EditedImage;
}

void
WallpaperBusinessLogic::setBackground (
        WallpaperITrans     *landscapeITrans,
        WallpaperITrans     *portraitITrans,
        WallpaperDescriptor *desc)
{
    Q_UNUSED (landscapeITrans);
    Q_UNUSED (portraitITrans);
    Q_UNUSED (desc);
}

bool
WallpaperBusinessLogic::supportsLandscape () const
{
    return true;
}

bool
WallpaperBusinessLogic::supportsPortrait () const
{
    return true;
}

void
WallpaperBusinessLogic::valueChanged ()
{

}

void
WallpaperBusinessLogic::editRequestArrived (
    QString a, QString b)
{
    Q_UNUSED (a);
    Q_UNUSED (b);
}

void
WallpaperBusinessLogic::startEditThreadEnded ()
{

}

void 
WallpaperBusinessLogic::directoryChanged (
        const QString &path)
{
    Q_UNUSED (path);
    //emit fileListChanged ();
}

void 
WallpaperBusinessLogic::fileChanged (
        const QString &path)
{
    Q_UNUSED (path);
}

