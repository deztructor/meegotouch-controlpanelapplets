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

/*********************************************************************************
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
