/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "wallpaperdescriptor.h"

#include <QUrl>
#include <QFileInfo>
#include <QImage>
#include <QPixmap>
#include <QPainter>

#include <MTheme>

#define THUMBNAIL_BG_COLOR "Red"

#define DEBUG
#include "../debug.h"

const QString dir = "";

WallpaperDescriptor::WallpaperDescriptor() :
    m_HasThumbnail (false),
    m_Cached (false),
    m_ThumbnailPixmap (100, 100)
{
    m_ThumbnailPixmap.fill (QColor(THUMBNAIL_BG_COLOR));
}

WallpaperDescriptor::WallpaperDescriptor (
        const WallpaperDescriptor &orig) :
    QObject (),
    m_Cached (false)
{
    // FIXME: What about the other fields?!
    m_ThumbnailPixmap = orig.m_ThumbnailPixmap;
    m_Filename        = orig.m_Filename;
    m_HasThumbnail    = orig.m_HasThumbnail;
    m_Url             = orig.m_Url;
}

WallpaperDescriptor::WallpaperDescriptor(
        const QString &filename) :
    m_Filename (filename),
    m_HasThumbnail (false),
    m_Cached (false),
    m_ThumbnailPixmap (100, 100)
{
    m_ThumbnailPixmap.fill (QColor(THUMBNAIL_BG_COLOR));
    m_Url = QUrl::fromLocalFile (filename);
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
    m_Cached       = false;
    m_Filename     = filename;
    m_Url          = QUrl::fromLocalFile (filename);

    m_ThumbnailPixmap.fill (QColor(THUMBNAIL_BG_COLOR));
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
    QString  decoded;

    m_Url.setEncodedUrl (urlString.toAscii());
    path = m_Url.path();
    decoded = path.toUtf8();

    #if 1
    SYS_DEBUG ("---------------------------------------------------");
    qDebug() << "path = " << path;
    SYS_DEBUG ("*** mystuff      = %s", SYS_STR(decoded));
    SYS_DEBUG ("*** urlString    = %s", SYS_STR(urlString));
    SYS_DEBUG ("*** scheme       = %s", SYS_STR(m_Url.scheme()));
    SYS_DEBUG ("*** path         = %s", SYS_STR(path));
    #endif
    if (m_Url.scheme() != "file") {
        SYS_WARNING ("Only local files are supported yet.");
        return;
    }

    m_HasThumbnail = false;
    m_Cached       = false;
    m_Filename     = path;
}

QString
WallpaperDescriptor::basename () const 
{
    QFileInfo fileInfo (m_Filename);

    return fileInfo.baseName();
}

/*!
 * \returns The filename extension of the filename. 
 *
 * The filename extension is used when the applet creates a new file to save the
 * edited image. The edited file will use the same extension used in the
 * original filename.
 */
QString
WallpaperDescriptor::extension () const 
{
    QFileInfo fileInfo (m_Filename);

    return fileInfo.suffix();
}

#ifdef SUPPORT_IMAGE_THUMBNAILS
QImage 
WallpaperDescriptor::thumbnail()
{
    return m_Thumbnail;
}
#endif

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
        m_Cached = false;
        return;
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
    #ifdef LOTDEBUG
    SYS_DEBUG ("*** m_Filename = %s", SYS_STR(m_Filename));
    SYS_DEBUG ("*** m_MimeType = %s", SYS_STR(m_MimeType));
    #endif
    /*
     * If some necessary information is missing.
     */
    if (m_MimeType.isEmpty() || m_Filename.isEmpty()) {
        return;
    }

    /*
     * If the thumbnailer is already initiated.
     */
    if (m_Thumbnailer != 0) {
        return;
    }

    /*
     * If we already has the thumbnail.
     */
    if (m_HasThumbnail) {
        return;
    }

    SYS_DEBUG ("Creating the thumbnailer and connecting to its signals.");
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

    #ifdef USE_PAINTER
    m_Thumbnailer->request (urisList, mimeList);
    #else
    m_Thumbnailer->request (urisList, mimeList, true);
    #endif
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

    #ifdef LOTDEBUG
    SYS_DEBUG ("Got thumbnail for %s",  SYS_STR(m_Filename));
    SYS_DEBUG ("*** thumbnailUri = %s", SYS_STR(thumbnailUri.toString()));
    #endif

    #ifdef USE_PAINTER
    QPixmap thumb (thumbnailUri.toLocalFile());
    QPainter painter (&m_ThumbnailPixmap);
    qreal    ratio1, ratio2, ratio;

    ratio1 = 100.0 / thumb.width ();
    ratio2 = 100.0 / thumb.height ();
    ratio = ratio1 > ratio2 ? ratio1 : ratio2;

    #ifdef LOTDEBUG
    SYS_DEBUG ("*** w      = %d", thumb.width());
    SYS_DEBUG ("*** h      = %d", thumb.height());
    SYS_DEBUG ("*** ratio1 = %g", ratio1);
    SYS_DEBUG ("*** ratio2 = %g", ratio2);
    SYS_DEBUG ("*** ratio  = %g", ratio);
    #endif
    painter.drawPixmap (
                0, 0,
                thumb.width() * ratio,
                thumb.height() * ratio,
                thumb);
    #else
    m_ThumbnailPixmap = pixmap;
    #endif

    #ifdef SUPPORT_IMAGE_THUMBNAILS
    m_Thumbnail = pixmap.toImage();
    #endif

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
    /*
     * The thumbnailer failed. Let's see if we can do something about it.
     * FIXME: In the future maybe the cache should return a boolean and we
     * could set this descriptor to invalid... 
     */
    cache();
    if (!m_Cached) {
        m_HasThumbnail = false;
        m_ThumbnailPixmap = QPixmap (100, 100);
        m_ThumbnailPixmap.fill (QColor(THUMBNAIL_BG_COLOR));
        return;
    }

    m_ThumbnailPixmap = m_Pixmap.scaled (100, 100);
    m_HasThumbnail = true;

    #ifdef SUPPORT_IMAGE_THUMBNAILS
    m_Thumbnail = pixmap.toImage();
    #endif

    unCache ();
    
    emit thumbnailLoaded (this);
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

/*!
 * \returns true if this is the current wallpaper.
 */
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

/*
 * Returns a suggested file name that is used when a modified image file is
 * saved.
 */
QString 
WallpaperDescriptor::suggestedOutputFilename (
        M::Orientation orientation,
        int            ver) const
{
    QString retval;
    QString outputExtension;

    if (ver < 0)
        ver = version ();

    /*
     * There are some extensions we can not support for writing.
     */
    outputExtension = extension();
    if (QString::compare(outputExtension, "gif", Qt::CaseInsensitive) == 0 ||
        QString::compare(outputExtension, "bpm", Qt::CaseInsensitive) == 0 ||
        QString::compare(outputExtension, "pgm", Qt::CaseInsensitive) == 0)
        outputExtension = "jpeg";

    switch (orientation) {
        case M::Landscape:
            retval = "-landscape.";
            break;

        case M::Portrait:
            retval = "-portrait.";
            break;
    }

    retval = basename() + retval + 
        QString::number(ver) + "." + outputExtension;

    return retval;
}

/*
 * Returns the original image file that was loaded, modified and saved into a
 * new file. For this class the original filename is the only filename.
 *
 * FIXME: What if the original file is deleted?
 */
QString 
WallpaperDescriptor::originalImageFile (
        M::Orientation orientation) const
{
    Q_UNUSED (orientation);

    return filename ();
}

/*
 * This function always return true, only the inherited
 * WallpaperCurrentDescriptor has tests in it.
 */
bool
WallpaperDescriptor::valid () const
{
    return true;
}
