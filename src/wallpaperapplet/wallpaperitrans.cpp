/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "wallpaperitrans.h"

#include <QSize>
#include <QPixmap>
#include <QPointF>

#define DEBUG
#include "../debug.h"

WallpaperITrans::WallpaperITrans () :
    m_Scale (1.0)
{
}

WallpaperITrans &
WallpaperITrans::operator= (
        const WallpaperITrans &rhs)
{
    if (this != &rhs) {
        m_Scale = rhs.m_Scale;
    }

    return *this;
}

const int 
WallpaperITrans::operator* (
        const int i)
{
    SYS_DEBUG ("m_Scale = %f", m_Scale);
    return m_Scale * i;
}

void
WallpaperITrans::modScale (
        int  i)
{
    m_Scale = m_Scale + (i / 1200.0);
    if (m_Scale < 0.1)
        m_Scale = 0.1;
    
    SYS_DEBUG ("m_Scale = %f", m_Scale);
}

