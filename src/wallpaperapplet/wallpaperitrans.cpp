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

#include "wallpaperitrans.h"

#include <QSize>
#include <QPixmap>
#include <QPointF>

//#define DEBUG
#include "../debug.h"

WallpaperITrans::WallpaperITrans () :
    m_Orientation (M::Landscape),
    m_Scale (1.0)
{
}

WallpaperITrans::WallpaperITrans (
        const WallpaperITrans &orig) :
        QObject ()
{
    m_Orientation    = orig.m_Orientation;
    m_Scale          = orig.m_Scale;
    m_Offset         = orig.m_Offset;
    m_ExpectedSize   = orig.m_ExpectedSize;
}


WallpaperITrans &
WallpaperITrans::operator= (
        const WallpaperITrans &rhs)
{
    if (this != &rhs) {
        m_Orientation    = rhs.m_Orientation;
        m_Scale          = rhs.m_Scale;
        m_Offset         = rhs.m_Offset;
        m_ExpectedSize   = rhs.m_ExpectedSize;
    }

    return *this;
}

int 
WallpaperITrans::operator* (
        const int i) const
{
    //SYS_DEBUG ("m_Scale = %f", m_Scale);
    return m_Scale * i;
}

WallpaperITrans &
WallpaperITrans::operator+= (
        const QPointF &rhs)
{
    m_Offset += rhs;

    return *this;
}

int
WallpaperITrans::x () const
{
    return (int) m_Offset.x ();
}

int
WallpaperITrans::y () const
{
    return (int) m_Offset.y ();
}

void
WallpaperITrans::modScale (
        int  i)
{
    m_Scale = m_Scale + (i / 1200.0);
    if (m_Scale < 0.1)
        m_Scale = 0.1;
}

qreal
WallpaperITrans::scale () const
{
    return m_Scale;
}

void
WallpaperITrans::setExpectedSize (
        const QSize &size)
{
    SYS_DEBUG ("*** size = %dx%d", size.width(), size.height());
    m_ExpectedSize = size;
}

void
WallpaperITrans::setOffset (
        const QPointF &offset)
{
    m_Offset = offset;
}

void 
WallpaperITrans::setScale (
        qreal scale)
{
    m_Scale = scale;
}

QSize 
WallpaperITrans::expectedSize () const
{
    return m_ExpectedSize;
}

int
WallpaperITrans::expectedWidth () const
{
    return m_ExpectedSize.width();
}

int
WallpaperITrans::expectedHeight () const
{
    return m_ExpectedSize.height();
}

/*!
 * Returns the orientation set for the image transformation. The orientation is
 * needed for the file saving, a separate portrait and landscape image file will
 * be saved.
 */
M::Orientation 
WallpaperITrans::orientation () const
{
    return m_Orientation;
}

void 
WallpaperITrans::setOrientation (
        M::Orientation orientation)
{
    m_Orientation = orientation;
}

