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
        const QString  &filename,
        QObject        *parent) : 
    QObject (parent),
    m_Priv (new WallpaperDescriptorPrivate (filename))
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

