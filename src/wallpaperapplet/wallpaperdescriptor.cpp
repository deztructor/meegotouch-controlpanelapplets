/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "wallpaperdescriptor.h"

#include <QUrl>
#include <QFileInfo>
#include <QImage>
#include <QPixmap>

#define DEBUG
#include "../debug.h"

const QString dir = "";

WallpaperDescriptor::WallpaperDescriptor()
{
    m_ImageLoaded = false;
    m_Cached = false;
}

WallpaperDescriptor::WallpaperDescriptor(
        const WallpaperDescriptor &orig) :
    QObject ()
{
    m_ImageLoaded = false;
    m_Cached = false;
    m_Filename = orig.m_Filename;
}

WallpaperDescriptor::WallpaperDescriptor(
        const QString &filename) :
    m_Filename (filename)
{
    m_ImageLoaded = false;
    m_Cached = false;
}

WallpaperDescriptor::~WallpaperDescriptor()
{
}

void
WallpaperDescriptor::setFilename (
        const QString &filename)
{
    m_ImageLoaded = false;
    m_Cached = false;
    m_Filename = filename;
}

QString
WallpaperDescriptor::filename () const
{
    return m_Filename;
}

void
WallpaperDescriptor::setTitle (
        const QString &title)
{
    m_Title = title;
}

QString
WallpaperDescriptor::title () const
{
    if (m_Title.isEmpty())
        return basename ();

    return m_Title;
}

void
WallpaperDescriptor::setUrl (
        const QString &urlString)
{
    QUrl url (urlString);
    QString  path;

    path = url.encodedPath ();
    #if 0
    SYS_DEBUG ("*** urlString    = %s", SYS_STR(urlString));
    SYS_DEBUG ("*** scheme       = %s", SYS_STR(url.scheme()));
    SYS_DEBUG ("*** path         = %s", SYS_STR(path));
    #endif
    if (url.scheme() != "file") {
        SYS_WARNING ("Only local files are supported yet.");
        return;
    }

    m_ImageLoaded = false;
    m_Cached = false;
    m_Filename = path;
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
    QImage image;
    bool   success;

    if (m_ImageLoaded)
        return;
    SYS_DEBUG ("Lom_WallpaperBusinessLogic->editedImage()ading : %s", SYS_STR(filename()));

    success = image.load (filename());
    if (!success) {
        SYS_WARNING ("The image was not loaded from %s", SYS_STR(filename()));
    }

    m_Thumbnail = image.scaled (100, 100, Qt::KeepAspectRatioByExpanding);
    m_ImageLoaded = true;
}

QImage 
WallpaperDescriptor::thumbnail()
{
    return m_Thumbnail;
}

bool 
WallpaperDescriptor::isImageLoaded ()
{
    return m_ImageLoaded;
}

void 
WallpaperDescriptor::cache ()
{
    if (m_Cached)
        return;

    m_Pixmap.load (filename());
    m_Cached = true;
}

void
WallpaperDescriptor::unCache ()
{
    if (!m_Cached)
        return;

    m_Pixmap = QPixmap();
    m_Cached = false;
}

QPixmap
WallpaperDescriptor::pixmap ()
{
    cache();
    return m_Pixmap;
}

QPixmap 
WallpaperDescriptor::scaled (
        QSize  size)
{
    cache ();
    return m_Pixmap.scaled (size, Qt::KeepAspectRatioByExpanding);
}

