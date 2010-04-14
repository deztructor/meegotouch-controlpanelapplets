/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "wallpaperdescriptor.h"

#include <QFileInfo>

const QString dir = "";

WallpaperDescriptor::WallpaperDescriptor()
{
}

WallpaperDescriptor::WallpaperDescriptor(
        const WallpaperDescriptor &orig) :
    QObject ()
{
    m_Filename = orig.m_Filename;
}

WallpaperDescriptor::WallpaperDescriptor(
        const QString &filename) :
    m_Filename (filename)
{
}

WallpaperDescriptor::~WallpaperDescriptor()
{
}

void
WallpaperDescriptor::setFilename (
        const QString &filename)
{
    m_Filename = filename;
}

QString
WallpaperDescriptor::filename () const
{
    return m_Filename;
}

QString
WallpaperDescriptor::basename () const 
{
    QFileInfo fileInfo (m_Filename);

    return fileInfo.baseName();
}
