/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef WALLPAPERITRANS_H
#define WALLPAPERITRANS_H

#include <QObject>
#include <QPointF>
#include <QSize>
#include <MApplication>

class QSize;
class QPixmap;


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
    int operator* (const int i);
    WallpaperITrans &operator+= (const QPointF &rsh);

    qreal x () const;
    qreal y () const;
    void modScale (int i);
    
    void setExpectedSize (const QSize &size);
    QSize expectedSize () const;
    int expectedWidth () const;
    int expectedHeight () const;

    M::Orientation orientation () const;
    void setOrientation (M::Orientation orientation);
    
private:
    M::Orientation   m_Orientation;
    qreal            m_Scale;
    QPointF          m_Offset;
    QSize            m_ExpectedSize;
};
#endif
