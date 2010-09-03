/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "wallpaperinfoheader.h"

#include <MLabel>

#include <QGraphicsLinearLayout>
#include <QPixmap>

#define DEBUG
#include "../debug.h"

WallpaperInfoHeader::WallpaperInfoHeader (
        QGraphicsWidget *parent) :
    MStylableWidget (parent)
{
    QGraphicsLinearLayout    *HBox;

    SYS_DEBUG ("");

    m_Label = new MLabel;
    m_Label->setObjectName ("WallpaperInfoHeaderLabel");
    m_Label->setWordWrap (true);

    HBox = new QGraphicsLinearLayout (Qt::Horizontal);
    HBox->addItem (m_Label);

    setLayout (HBox);
    retranslateUi ();
}

WallpaperInfoHeader::~WallpaperInfoHeader ()
{
}


void
WallpaperInfoHeader::retranslateUi ()
{
    //% "Turn device to set wallpaper in another orientation"
    m_Label->setText(qtTrId ("qtn_wall_info_turn_device"));
}


