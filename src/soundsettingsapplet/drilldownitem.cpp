/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of meegotouch-controlpanelsoundsettingsapplet.
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
#include "drilldownitem.h"

#include <MContainer>
#include <MLabel>
#include <MImageWidget>
#include <QGraphicsGridLayout>
#include <QGraphicsLinearLayout>
#include <MLinearLayoutPolicy>

#define DEBUG
#define WARNING
#include "../debug.h"

DrillDownItem::DrillDownItem (
        MBasicListItem::ItemStyle  style, 
        QGraphicsItem             *parent) : 
    MBasicListItem (style, parent),
    m_TitleStyleName ("CommonTitleInverted")
{
}

QGraphicsLayout *
DrillDownItem::createLayout()
{
    QGraphicsGridLayout *layout;
    MLabel              *titleLabel;
    MLabel              *subTitleLabel;
    MImageWidget        *iconWidget;
    MImageWidget        *drillIconWidget;
    
    layout = new QGraphicsGridLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    titleLabel =  titleLabelWidget ();
    subTitleLabel = subtitleLabelWidget ();
    iconWidget = imageWidget();

    drillIconWidget = new MImageWidget ("icon-m-common-drilldown-arrow-inverse");
    drillIconWidget->setStyleName ("CommonDrillDownIcon");

    titleLabel->setStyleName(m_TitleStyleName);
    subTitleLabel->setStyleName("CommonSubTitleInverted");

    switch (itemStyle()) {
        case TitleWithSubtitle:
            /*
             * The title label.
             */
            layout->addItem (titleLabel, 0, 0);
            layout->setAlignment(titleLabel, Qt::AlignLeft | Qt::AlignVCenter);
            /*
             * The sub-title label.
             */
            layout->addItem (subTitleLabel, 1, 0);
            layout->setAlignment (subTitleLabel, Qt::AlignLeft | Qt::AlignVCenter);
            /*
             * The drill down icon.
             */
            layout->addItem(drillIconWidget, 0, 1, 2, 1);
            layout->setAlignment (drillIconWidget, Qt::AlignVCenter | Qt::AlignRight);
            break;

        case IconWithTitleAndSubtitle:
            /*
             * The left side icon.
             */
            layout->addItem (iconWidget, 0, 0, 2, 1);
            layout->setAlignment (iconWidget, Qt::AlignVCenter | Qt::AlignRight);
            /*
             * The title label.
             */
            layout->addItem (titleLabel, 0, 1);
            layout->setAlignment (titleLabel, Qt::AlignLeft | Qt::AlignVCenter);
            /*
             * The sub-title label.
             */
            layout->addItem (subTitleLabel, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
            /*
             * The drill down icon.
             */
            layout->addItem(drillIconWidget, 0, 2, 2, 1);
            layout->setAlignment (drillIconWidget, Qt::AlignVCenter | Qt::AlignRight);
            break;

        case IconWithTitle:
            /*
             * The left side icon.
             */
            layout->addItem (iconWidget, 0, 0);
            layout->setAlignment (iconWidget, Qt::AlignVCenter | Qt::AlignRight);
            /*
             * The title label.
             */
            layout->addItem (titleLabel, 0, 1);
            layout->setAlignment (titleLabel, Qt::AlignLeft | Qt::AlignVCenter);
            /*
             * The drill down icon.
             */
            layout->addItem(drillIconWidget, 0, 2);
            layout->setAlignment (drillIconWidget, Qt::AlignVCenter | Qt::AlignRight);
            break;

        default:
            SYS_WARNING ("itemStyle not supported.");
    }

    setStyleName ("CommonPanelInverted");
    return layout;
}

void 
DrillDownItem::setTitleStyleName (
        const QString &styleName)
{
    m_TitleStyleName = styleName;
}

/******************************************************************************
 *
 */
MLabel *
addTitleLabel (
        QGraphicsWidget     *parent,
        MLinearLayoutPolicy *targetPolicy,
        const char          *panelStyleName,
        const char          *labelStyleName)
{
    MContainer              *container;
    QGraphicsLinearLayout   *layout;
    MLabel                  *label;

    label = new MLabel;
    label->setStyleName (labelStyleName);

    container = new MContainer (parent);
    container->setContentsMargins (0., 0., 0., 0.);
    container->setStyleName (panelStyleName);
    container->setHeaderVisible (false);

    layout = new QGraphicsLinearLayout (Qt::Horizontal);
    layout->setContentsMargins (0., 0., 0., 0.);
    layout->addItem (label);

    container->centralWidget()->setLayout (layout);

    targetPolicy->addItem (container);
    return label;
}

MLabel *
addTitleLabel (
        QGraphicsWidget     *parent,
        MLinearLayoutPolicy *targetPolicy,
        const char          *labelStyleName)
{
    Q_UNUSED (parent);
    MLabel                  *label;

    label = new MLabel;
    label->setStyleName (labelStyleName);

    targetPolicy->addItem (label);
    return label;
}



