/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of meegotouch-controlpanelapplets.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include "percentagecontainer.h"

#include <QGraphicsLinearLayout>
#include <MImageWidget>
#include <MLabel>
#include <MStylableWidget>

#undef DEBUG
#define WARNING
#include "../debug.h"

PercentageContainer::PercentageContainer(
        const QString  &text,
        MImageWidget   *image,
        MWidget        *parent) :
    MContainer (parent),
    m_MainLayout (0),
    m_Image (image),
    m_TextLabel (0),
    m_SubTextLabel (0)
{
    SYS_DEBUG ("*** text = %s", SYS_STR(text));

    setStyleName ("CommonTextFrameInverted");
    setObjectName ("PercentageContainer");

    m_TextLabel = new MLabel (text);
    m_TextLabel->setStyleName ("CommonSingleTitleInverted");

    setHeaderVisible (false);
    setLayout ();
}


void
PercentageContainer::setText (
        const QString &text)
{
    m_TextLabel->setText (text);
}

void 
PercentageContainer::updateCapacity (
        const int value)
{
    QString text;
    //% "Battery level \%L1\%"
    text = qtTrId ("qtn_ener_battery_level").arg (value);
    SYS_DEBUG ("*** text = %s", SYS_STR(text));
    m_TextLabel->setText(text);
}

void 
PercentageContainer::updateRemainingChargingTime (
        int    ChTime)
{
    SYS_DEBUG ("*** ChTime            = %d", ChTime);
    SYS_DEBUG ("*** m_SubTextLabel    = %p", m_SubTextLabel);

    if (ChTime <= 0 && m_SubTextLabel) {
        m_SubTextLabel->deleteLater();
        m_SubTextLabel = 0;
        return;
    } 
    
    if (ChTime > 0 && !m_SubTextLabel) {
        m_SubTextLabel = new MLabel;
        m_SubTextLabel->setObjectName ("SubTextLabel");
        m_SubTextLabel->setStyleName ("CommonSubTitleInverted");
        m_MainLayout->addItem (m_SubTextLabel);
    }

    if (ChTime > 0) {
        int minutes = ChTime / 60;
        m_SubTextLabel->setText (
                qtTrId ("qtn_ener_charging_estimate", minutes).arg(minutes));
    }
}

/*
 * This method will create all the internal widgets. 
 */
void 
PercentageContainer::setLayout()
{
    QGraphicsLinearLayout *layout;

    m_MainLayout = new QGraphicsLinearLayout (Qt::Vertical);
    m_MainLayout->setContentsMargins (0., 0., 0., 0.);
    m_MainLayout->setSpacing (0.);

    layout = new QGraphicsLinearLayout (Qt::Horizontal);
    layout->setContentsMargins (0., 0., 0., 0.);
    layout->setSpacing (0.);

    // add the widgets
    layout->addItem (m_Image);
    layout->setAlignment (m_Image, Qt::AlignLeft | Qt::AlignVCenter);
    layout->addItem (m_TextLabel);
    layout->setAlignment (m_TextLabel, Qt::AlignLeft | Qt::AlignVCenter);

    m_MainLayout->addItem (layout);

    // set the layout
    centralWidget ()->setLayout (m_MainLayout);
}

