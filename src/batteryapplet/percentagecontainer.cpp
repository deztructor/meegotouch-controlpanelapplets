/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
#include "../debug.h"

PercentageContainer::PercentageContainer(
    const QString  &text,
    MImageWidget *image,
    MWidget      *parent) :
    MContainer (parent),
        m_Image (image),
        m_TextLabel (0)
{
    SYS_DEBUG ("*** text = %s", SYS_STR(text));

    setStyleName ("CommonPanel");
    setObjectName ("PercentageContainer");

    m_TextLabel = new MLabel (text);
    m_TextLabel->setObjectName ("CommonSingleTitle");

    setHeaderVisible (false);
    setLayout ();
}


void
PercentageContainer::setText (
        const QString &text)
{
    m_TextLabel->setText (text);
}

void PercentageContainer::updateCapacity(
        const int value)
{
    QString text;
    //% "Battery level \%L1\%"
    text = qtTrId ("qtn_ener_battery_level").arg (value);
    SYS_DEBUG ("*** text = %s", SYS_STR(text));
    m_TextLabel->setText(text);
}

void PercentageContainer::setLayout()
{
    MStylableWidget       *stretcher;
    QGraphicsLinearLayout *mainLayout;
    QGraphicsLinearLayout *layout;

    mainLayout = new QGraphicsLinearLayout (Qt::Vertical);
    mainLayout->setContentsMargins (0., 0., 0., 0.);
    mainLayout->setSpacing (0.);

    layout = new QGraphicsLinearLayout (Qt::Horizontal);
    layout->setContentsMargins (0., 0., 0., 0.);
    layout->setSpacing (0.);

    stretcher = new MStylableWidget ();
    stretcher->setObjectName ("CommonSpacer");
    
    // add the widgets
    layout->addItem (m_Image);
    layout->setAlignment (m_Image, Qt::AlignVCenter);
    layout->addItem (m_TextLabel);
    layout->setAlignment (m_TextLabel, Qt::AlignVCenter);

    mainLayout->addItem (layout);
    mainLayout->addItem (stretcher);

    // set the layout
    centralWidget ()->setLayout (mainLayout);
}

