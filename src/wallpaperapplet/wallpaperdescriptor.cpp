/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "wallpaperdescriptor.h"

#include <QFileInfo>
#include <QImage>

#define DEBUG
#include "../debug.h"

const QString dir = "";

WallpaperDescriptor::WallpaperDescriptor()
{
    m_ImageLoaded = false;
}

WallpaperDescriptor::WallpaperDescriptor(
        const WallpaperDescriptor &orig) :
    QObject ()
{
    m_ImageLoaded = false;
    m_Filename = orig.m_Filename;
}

WallpaperDescriptor::WallpaperDescriptor(
        const QString &filename) :
    m_Filename (filename)
{
    m_ImageLoaded = false;
}

WallpaperDescriptor::~WallpaperDescriptor()
{
}

void
WallpaperDescriptor::setFilename (
        const QString &filename)
{
    m_ImageLoaded = false;
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

QString
WallpaperDescriptor::extension () const 
{
    QFileInfo fileInfo (m_Filename);

    return fileInfo.suffix();
}

void
WallpaperDescriptor::loadImage ()
{
    bool success;

    if (m_ImageLoaded)
        return;
    SYS_DEBUG ("Loading : %s", SYS_STR(filename()));

    success = m_Image.load (filename());
    if (!success) {
        SYS_WARNING ("The image was not loaded from %s", SYS_STR(filename()));
    }

    m_Thumbnail = m_Image.scaled(100, 100, Qt::KeepAspectRatio);
    m_Image = m_Image.scaled(800, 480, Qt::KeepAspectRatio);
    m_ImageLoaded = true;
}

QImage 
WallpaperDescriptor::thumbnail()
{
    return m_Thumbnail;
}

QImage 
WallpaperDescriptor::image()
{
    return m_Image;
}

bool 
WallpaperDescriptor::isImageLoaded ()
{
    return m_ImageLoaded;
}
