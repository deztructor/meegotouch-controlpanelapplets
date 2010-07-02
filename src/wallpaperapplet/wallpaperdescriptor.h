/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#ifndef WALLPAPERDESCRIPTOR_H
#define WALLPAPERDESCRIPTOR_H

#include <QObject>
#include <QMetaType>
#include <QImage>
#include <QPixmap>
#include <QUrl>
#include <QPointer>
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

// If this macro is defined the class will support the QPixmap class thumbnails.
// I believe in the current version of the Meego Library it is not necessary,
// but some versions will need this in order to show the image in the lists.
#undef SUPPORT_IMAGE_THUMBNAILS

// If this macro is enabled the thumbnail file will be loaded by the class
// itself. This way we can try to eliminate the aspect ratio distortion, but
// since the current version of the thumbler does not provide such a flavor (or
// it is just me who did not find such a flavor) using our own implementation 
// to paint will not solve the problem.
#undef USE_PAINTER

/*!
 * This class is used to represent a potential wallpaper image with all its
 * attributes. The WallpaperList is using this class to visualize the wallpaper
 * images.
 */
class Q_DECL_EXPORT WallpaperDescriptor : public QObject {
    Q_OBJECT
public:
    WallpaperDescriptor ();
    WallpaperDescriptor (const QString &filename);
    WallpaperDescriptor (const WallpaperDescriptor &orig);
    ~WallpaperDescriptor ();

    void setFilename (const QString &filename);
    QString filename () const;

    void setTitle (const QString &title);
    QString title () const;
    
    virtual bool isCurrent () const;
    virtual int version () const;
    virtual QString suggestedOutputFilename (
            M::Orientation orientation,
            int            ver = -1) const;
    virtual QString originalImageFile (M::Orientation orientation) const;
    virtual bool valid () const;

    void setUrl (const QString &urlString);

    void setMimeType (const QString &newMimeType);
    QString mimeType () const;

    QString basename () const;
    QString extension () const;

    bool isThumbnailLoaded ();
    #ifdef SUPPORT_IMAGE_THUMBNAILS
    QImage thumbnail ();
    #endif
    QPixmap thumbnailPixmap ();

    void cache ();
    void unCache ();
    QPixmap pixmap ();
    QPixmap scaled (QSize size);

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
    
private:
    QPointer<Thumbnailer> m_Thumbnailer;
    QUrl                  m_Url;
    QString               m_Filename;
    QString               m_Title;
    QString               m_MimeType;
    bool                  m_HasThumbnail;
    bool                  m_Cached;
    QPixmap               m_ThumbnailPixmap;
    QPixmap               m_Pixmap;
    #ifdef SUPPORT_IMAGE_THUMBNAILS
    QImage                m_Thumbnail;
    #endif

    #ifdef UNIT_TEST
    friend class Ut_WallpaperDescriptor;
    friend class Ft_WallpaperDescriptor;
    friend class Ut_WallpaperModel;
    #endif
};

Q_DECLARE_METATYPE(WallpaperDescriptor)
Q_DECLARE_METATYPE(WallpaperDescriptor *)

#endif

