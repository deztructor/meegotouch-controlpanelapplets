/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#ifndef WALLPAPERDESCRIPTOR_H
#define WALLPAPERDESCRIPTOR_H

#include <QObject>
#include <QPointer>
#include <QMetaType>
#include <QImage>
#include <QPixmap>
#include <QUrl>
#include <QVector>
#include <MApplication>


/*
 * In the functional tests we use the real thing, in the unit tests we use the
 * stubbed version. 
 */
#if defined(UNIT_TEST) && !defined(FUNCTIONAL_TEST)
#  include "thumbnailerstub.h"
#else
#  include <Thumbnailer>
#endif

using namespace Thumbnails;

class QString;


// If this macro is enabled the thumbnail file will be loaded by the class
// itself. This way we can try to eliminate the aspect ratio distortion, but
// since the current version of the thumbler does not provide such a flavor (or
// it is just me who did not find such a flavor) using our own implementation 
// to paint will not solve the problem.
#undef USE_PAINTER


class Q_DECL_EXPORT Image : public QObject {
    Q_OBJECT

public:
    Image ();
    Image(const Image&);
    Image &operator= (const Image &rhs);
    
    void reset ();

    void setFilename (const QString &fileName);
    QString filename () const;

    void setMimeType (const QString &mimeType);
    QString mimeType () const;

    void setImageID (const QString &imageID);
    QString imageID () const;
    
    void setUrl (const QString &urlString);
    QUrl url () const {return m_Url;};

    void setTitle (const QString &title);
    QString title () const;

    QString basename () const;
    QString extension () const;

    bool hasThumbnail () const { return m_HasThumbnail; };
    QPixmap thumbnailPixmap () const {return m_ThumbnailPixmap;};
    bool setThumbnailPixmap (const QPixmap &pixmap); 
    bool thumbnail (bool force = false);

    void cache (bool threadSafe = false);
    void unCache ();

    QPixmap pixmap ();
    QPixmap scaled (QSize size);

private:
    QString   m_Filename;
    QString   m_MimeType;
    QString   m_ImageID;
    QString   m_Title;
    bool      m_Cached;
    QUrl      m_Url;  
    QPixmap   m_ThumbnailPixmap;
    bool      m_HasThumbnail;
    QPixmap   m_Pixmap;
    QImage   *m_Image;

    #ifdef UNIT_TEST
    friend class Ut_WallpaperDescriptor;
    friend class Ft_WallpaperDescriptor;
    friend class Ut_WallpaperModel;
    #endif
};

/*!
 * This class is used to represent a potential wallpaper image with all its
 * attributes. The WallpaperList is using this class to visualize the wallpaper
 * images.
 */
class Q_DECL_EXPORT WallpaperDescriptor : public QObject {
    Q_OBJECT

public:

    typedef enum {
        Landscape   = 0,
        Portrait,
        OriginalLandscape,
        OriginalPortrait,
        NVariants
    } ImageVariant;

    WallpaperDescriptor (
            QObject *parent = NULL);
    
    WallpaperDescriptor (const QString &filename);
    WallpaperDescriptor (const WallpaperDescriptor &orig);
    ~WallpaperDescriptor ();

    void setFilename (
            const QString &filename,
            ImageVariant   variant = WallpaperDescriptor::Landscape);
    QString filename (
            ImageVariant   variant = WallpaperDescriptor::Landscape) const;
    
    void setMimeType (
            const QString &mimeType,
            ImageVariant   variant = WallpaperDescriptor::Landscape);
    QString mimeType (
            ImageVariant   variant = WallpaperDescriptor::Landscape) const;


    void setImageID  (
            const QString &imageID,
            ImageVariant   variant = WallpaperDescriptor::Landscape);
    QString imageID (
            ImageVariant   variant = WallpaperDescriptor::Landscape) const;

    void setUrl (
            const QString &urlString,
            ImageVariant   variant = WallpaperDescriptor::Landscape);


    void setTitle (
            const QString &title,
            ImageVariant   variant = WallpaperDescriptor::Landscape);
    QString title (
            ImageVariant   variant = WallpaperDescriptor::Landscape) const;
    
    QString basename (
            ImageVariant   variant = WallpaperDescriptor::Landscape) const;
    QString extension (
            ImageVariant   variant = WallpaperDescriptor::Landscape) const;
    
    virtual bool isCurrent () const;
    virtual int version () const;
    
    virtual QString suggestedOutputFilename (
            M::Orientation orientation,
            int            ver = -1) const;
    virtual QString suggestedOutputMimeType (
            M::Orientation orientation);

    virtual QString originalImageFile (M::Orientation orientation) const;
    virtual bool valid () const;

    bool isThumbnailLoaded (
            ImageVariant   variant = WallpaperDescriptor::Landscape) const;

    QPixmap thumbnailPixmap () const;

    void cache (
            ImageVariant   variant = WallpaperDescriptor::Landscape);
    void unCache (
            ImageVariant   variant = WallpaperDescriptor::Landscape);
    
    bool loading () const;
    void setLoading (bool loading = true);
    
    QPixmap pixmap (
            ImageVariant   variant = WallpaperDescriptor::Landscape);
    QPixmap scaled (
            QSize size,
            ImageVariant   variant = WallpaperDescriptor::Landscape);

    void loadAll (bool threadSafe = true);

public slots:
    void initiateThumbnailer ();

private slots:
    void thumbnailReady (
            QUrl         fileUri, 
            QUrl         thumbnailUri, 
            QPixmap      pixmap, 
            QString      flavor);
    void thumbnailError (
            QString      message,
            QUrl         url);
    void thumbnailLoadingFinished (
            int          left);

signals:
    void thumbnailLoaded (WallpaperDescriptor *desc);
    void changed (WallpaperDescriptor *desc);
    
protected:
    QVector<Image>        m_Images;
    QPointer<Thumbnailer> m_Thumbnailer;
    bool                  m_Loading;

    #ifdef UNIT_TEST
    friend class Ut_WallpaperDescriptor;
    friend class Ft_WallpaperDescriptor;
    friend class Ut_WallpaperModel;
    #endif
};

Q_DECLARE_METATYPE(WallpaperDescriptor)
Q_DECLARE_METATYPE(WallpaperDescriptor *)

#endif

