/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#ifndef WALLPAPERDESCRIPTOR_H
#define WALLPAPERDESCRIPTOR_H

#include <QObject>
#include <QMetaType>
#include <QImage>

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
    QString basename () const;

    void loadImage ();
    bool isImageLoaded ();
    QImage thumbnail ();
    QImage image ();

private:
    bool        m_ImageLoaded;
    QString     m_Filename;
    QImage      m_Image;
    QImage      m_Thumbnail;
};

Q_DECLARE_METATYPE(WallpaperDescriptor)
Q_DECLARE_METATYPE(WallpaperDescriptor *)

#endif
