/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "wallpaperdescriptor.h"

#include <QUrl>
#include <QFileInfo>
#include <QImage>
#include <QPixmap>
#include <Thumbnailer>

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
    QString  path;

    m_Url.setUrl (urlString);
    path = m_Url.encodedPath ();
    #if 0
    SYS_DEBUG ("*** urlString    = %s", SYS_STR(urlString));
    SYS_DEBUG ("*** scheme       = %s", SYS_STR(m_Url.scheme()));
    SYS_DEBUG ("*** path         = %s", SYS_STR(path));
    #endif
    if (m_Url.scheme() != "file") {
        SYS_WARNING ("Only local files are supported yet.");
        return;
    }

    m_ImageLoaded = false;
    m_Cached = false;
    m_Filename = path;

    //initiateThumbnailer ();
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

#if 0
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

    //m_Thumbnail = image.scaled (100, 100, Qt::KeepAspectRatioByExpanding);
    m_ImageLoaded = true;
}
#endif

QImage 
WallpaperDescriptor::thumbnail()
{
    return m_Thumbnail;
}

QPixmap
WallpaperDescriptor::thumbnailPixmap()
{
    return m_ThumbnailPixmap;
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

void
WallpaperDescriptor::setMimeType (
        const QString &mimeType)
{
    m_MimeType = mimeType;
    //initiateThumbnailer ();
}

void 
WallpaperDescriptor::initiateThumbnailer ()
{
   
    SYS_DEBUG ("*** MIME    = %s", SYS_STR(m_MimeType));
    SYS_DEBUG ("*** file    = %s", SYS_STR(m_Filename));
    if (m_MimeType.isEmpty() || m_Filename.isEmpty())
        return;

    SYS_DEBUG ("Initiating thumbnailer for %s", SYS_STR(m_Filename));
    m_Thumbnailer = new Thumbnailer;

    connect (m_Thumbnailer, SIGNAL(thumbnail(QUrl,QUrl,QPixmap,QString)),
            this, SLOT(thumbnailReady(QUrl,QUrl,QPixmap,QString)) );
    connect (m_Thumbnailer, SIGNAL(error(QString,QUrl)),
            this, SLOT(thumbnailError(QString,QUrl)) );
    connect (m_Thumbnailer, SIGNAL(finished(int)),
            this, SLOT(thumbnailLoadingFinished(int)));

    QList<QUrl> urisList;
    QStringList mimeList;

    urisList << m_Url;
    mimeList << m_MimeType;

    m_Thumbnailer->request (urisList, mimeList, true);
}

void 
WallpaperDescriptor::thumbnailReady (
            QUrl         fileUri, 
            QUrl         thumbnailUri, 
            QPixmap      pixmap, 
            QString      flavor)
{
    Q_UNUSED (fileUri);
    Q_UNUSED (thumbnailUri);
    Q_UNUSED (flavor);

    SYS_DEBUG ("Got thumbnail for %s",  SYS_STR(m_Filename));
    m_ThumbnailPixmap = pixmap;
    m_Thumbnail = pixmap.toImage();
}

void
WallpaperDescriptor::thumbnailError (
            QString      message,
            QUrl         url)
{
    Q_UNUSED (url);
    SYS_WARNING ("*** message = %s", SYS_STR(message));
}

void 
WallpaperDescriptor::thumbnailLoadingFinished (
            int          left)
{
    delete m_Thumbnailer;
}

