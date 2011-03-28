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
    
WallpaperBusinessLogic::WallpaperBusinessLogic ()
{

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
    desc->setTitle ("The image title1");

    testList << desc;

    desc = new WallpaperDescriptor;

    desc->setUrl ("file:///AnotherSimulatedDir/WithASimulatedFile.jpg");
    desc->setMimeType ("image/jpg");
    desc->setTitle ("The image title2");

    testList << desc;

    return testList;
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

void
WallpaperBusinessLogic::addImageFromGallery (
    QString image)
{
    Q_UNUSED (image);
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

