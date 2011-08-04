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

void
WallpaperBusinessLogic::currentWallpaper (
        QString   &currentFilePath,
        QString   &originalFilePath)
{
}

void 
WallpaperBusinessLogic::workerThreadFinishedSave ()
{
}

void
WallpaperBusinessLogic::gconfTimerSlot ()
{
}


void 
WallpaperBusinessLogic::workerThreadFinishedLoad ()
{
}

void
WallpaperBusinessLogic::portraitGConfChanged ()
{
}

void
WallpaperBusinessLogic::portraitHistoryChanged ()
{
}

QStringList
WallpaperBusinessLogic::history () const
{
    return QStringList();
}


