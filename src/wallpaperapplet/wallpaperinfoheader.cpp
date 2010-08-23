/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "wallpaperinfoheader.h"

#include <MLabel>

#include <QGraphicsLinearLayout>
#include <QPixmap>

#include "mwidgetcreator.h"
M_REGISTER_WIDGET_NO_CREATE(WallpaperInfoHeader)


#define DEBUG
#include "../debug.h"

WallpaperInfoHeader::WallpaperInfoHeader (
        QGraphicsWidget *parent) :
    MWidget (parent),
    m_Background (0)
{
    QGraphicsLinearLayout    *HBox;

    SYS_DEBUG ("");

    m_Label = new MLabel;
    m_Label->setObjectName ("WallpaperInfoHeaderLabel");
    m_Label->setWordWrap (true);

    HBox = new QGraphicsLinearLayout (Qt::Horizontal);
    HBox->addItem (m_Label);

    m_Background = MTheme::pixmapCopy ("top_fade");

    
    setLayout (HBox);
    retranslateUi ();
}

WallpaperInfoHeader::~WallpaperInfoHeader ()
{
    delete m_Background;
}


void
WallpaperInfoHeader::retranslateUi ()
{
    //% "Turn device to set wallpaper in another orientation"
    m_Label->setText(qtTrId ("qtn_wall_info_turn_device"));
}

void
WallpaperInfoHeader::paint (
        QPainter                       *painter,
        const QStyleOptionGraphicsItem *option,
        QWidget                        *widget)
{
    Q_UNUSED (option);
    Q_UNUSED (widget);

    const int margin = 10; // FIXME

    painter->drawPixmap (
            -margin, -margin,
            (int) geometry().width () + 2 * margin,
            (int) geometry().height() + 2 * margin,
            * m_Background);
}

