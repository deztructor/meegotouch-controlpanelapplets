/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef WALLPAPERITRANS_H
#define WALLPAPERITRANS_H

#include <QObject>

class QSize;
class QPixmap;
class QPointF;

/*!
 * A class to manipulate and save images for the meego control panel wallpaper 
 * applet.
 */
class WallpaperITrans : public QObject
{
    Q_OBJECT

public:
    WallpaperITrans ();

    WallpaperITrans &operator= (const WallpaperITrans &rhs);
    const int operator* (const int i);

    void modScale (int i);

private:
    qreal   m_Scale;
};
#endif
