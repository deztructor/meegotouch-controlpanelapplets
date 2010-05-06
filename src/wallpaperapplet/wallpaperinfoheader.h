/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef WALLPAPERINFOHEADER_H
#define WALLPAPERINFOHEADER_H

#include <QObject>
#include <MWidget>

class MImageWidget;
class MLabel;
class QPixmap;

class WallpaperInfoHeader : public MWidget
{
    Q_OBJECT

public:
    WallpaperInfoHeader (QGraphicsWidget *parent = 0);
    ~WallpaperInfoHeader ();

    virtual void paint (
            QPainter                       *painter,
            const QStyleOptionGraphicsItem *option,
            QWidget                        *widget = 0);
 
    virtual void retranslateUi ();

private:
    QPixmap          *m_Background;
    MImageWidget     *m_Icon;
    MLabel           *m_Label;
};

#endif
