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
#include <Thumbnailer>
#include <MApplication>

class QString;

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
    virtual QString suggestedOutputFilename (M::Orientation orientation) const;
    virtual QString originalImageFile (M::Orientation orientation) const;

    void setUrl (const QString &urlString);

    void setMimeType (const QString &mimeType);
    QString mimeType () const;

    QString basename () const;
    QString extension () const;

    bool isThumbnailLoaded ();
    QImage thumbnail ();
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
    QImage                m_Thumbnail;
    QPixmap               m_ThumbnailPixmap;
    bool                  m_Cached;
    QPixmap               m_Pixmap;
};

Q_DECLARE_METATYPE(WallpaperDescriptor)
Q_DECLARE_METATYPE(WallpaperDescriptor *)

#endif

