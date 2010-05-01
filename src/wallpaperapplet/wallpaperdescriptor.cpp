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

WallpaperDescriptor::WallpaperDescriptor() :
    m_HasThumbnail (false),
    m_Cached (false)
{
    m_HasThumbnail = false;
    m_Cached = false;
}

WallpaperDescriptor::WallpaperDescriptor(
        const WallpaperDescriptor &orig) :
    QObject (),
    m_HasThumbnail (false),
    m_Cached (false)
{
    m_Filename = orig.m_Filename;
}

WallpaperDescriptor::WallpaperDescriptor(
        const QString &filename) :
    m_Filename (filename),
    m_HasThumbnail (false),
    m_Cached (false)
{
}

WallpaperDescriptor::~WallpaperDescriptor()
{
}

void
WallpaperDescriptor::setFilename (
        const QString &filename)
{
    SYS_DEBUG ("*** filename = %s", SYS_STR(filename));
    Q_ASSERT (!filename.isEmpty());

    m_HasThumbnail = false;
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

    m_HasThumbnail = false;
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
WallpaperDescriptor::isThumbnailLoaded ()
{
    return m_HasThumbnail;
}

void 
WallpaperDescriptor::cache ()
{
    bool success;

    if (m_Cached)
        return;

    success = m_Pixmap.load (filename());
    if (!success) {
        SYS_WARNING ("Loading of %s has been failed.", SYS_STR(filename()));
        // FIXME: Well, we should not fail for a faulty image file...
        Q_ASSERT (false);
    }

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
        const QString &newMimeType)
{
    m_MimeType = newMimeType;
}

QString
WallpaperDescriptor::mimeType () const
{
    return m_MimeType;
}

/*!
 * This function will initiate the thumbnail generation. The thumbnail will be
 * loaded into the pixmap that is returned by the thumbnailPixmap() function. I
 * guess the pixmap will notify the 
 */
void 
WallpaperDescriptor::initiateThumbnailer ()
{
    if (m_MimeType.isEmpty() || m_Filename.isEmpty()) {
        SYS_WARNING ("Can not initiate thumbnailer for %s", 
                SYS_STR(title()));
        SYS_DEBUG ("*** MIME    = %s", SYS_STR(m_MimeType));
        SYS_DEBUG ("*** file    = %s", SYS_STR(m_Filename));
        return;
    }

    if (m_Thumbnailer != 0) {
        SYS_WARNING ("The thumbnailing already initated for %s", 
                SYS_STR(title()));
        return;
    }

    if (m_HasThumbnail) {
        SYS_WARNING ("Already has a thumbnail for %s", SYS_STR(title()));
        return;
    }

    SYS_DEBUG ("Initiating thumbnailer for %s", SYS_STR(m_Filename));
    SYS_DEBUG ("*** MIME    = %s", SYS_STR(m_MimeType));
    SYS_DEBUG ("*** file    = %s", SYS_STR(m_Filename));
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

/*!
 * This slot is called when the thumbnail has been generated.
 */
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
    SYS_DEBUG ("*** thumbnailUri = %s", SYS_STR(thumbnailUri.toString()));
    m_ThumbnailPixmap = pixmap;
    m_Thumbnail = pixmap.toImage();
    m_HasThumbnail = true;

    emit thumbnailLoaded (this);
}

/*!
 * This slot is called when the thumbler has been detected some error. We could
 * do something, but until we find some smart thing we can do we just print an
 * error message.
 */
void
WallpaperDescriptor::thumbnailError (
            QString      message,
            QUrl         url)
{
    Q_UNUSED (url);
    SYS_WARNING ("Failed to thumbnail %s: %s", 
            SYS_STR(title()),
            SYS_STR(message));
}

/*!
 * Called when the generation of the thumbnail is finished. This is the point
 * where we can destroy the thumbler object.
 */
void 
WallpaperDescriptor::thumbnailLoadingFinished (
            int          left)
{
    Q_UNUSED (left);
    delete m_Thumbnailer;
}

bool
WallpaperDescriptor::isCurrent () const
{
    return false;
}

/*!
 * Applications need to listen the wallpaper GConf items, so whenever we save a
 * file we need to modify the filename of the saved images. We could re-use the
 * filename, but then the GConf key would not be changed, so the applications
 * would not be able to recognize the change.
 * 
 * For this purpose we use a version number in the filenames. The
 * WallpaperDescriptor returns one default value for version number, the
 * WallpaperCurrentDescriptor on the other hand reads the version number from
 * the desktop file. 
 */
int
WallpaperDescriptor::version () const 
{
    return 0;
}

QString 
WallpaperDescriptor::suggestedOutputFilename (
        M::Orientation orientation) const
{
    QString retval;

    switch (orientation) {
        case M::Landscape:
            retval = "-landscape.";
            break;

        case M::Portrait:
            retval = "-portrait.";
            break;
    }

    retval = basename() + retval + 
        QString::number(version()) + "." + extension();

    return retval;
}

QString 
WallpaperDescriptor::originalImageFile (
        M::Orientation orientation) const
{
    Q_UNUSED (orientation);

    return filename ();
}
