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

#include <MWidgetController>
#include <MLabel>
#include <MImageWidget>
#include <QGraphicsGridLayout>
#include <QGraphicsLinearLayout>
#include <MLinearLayoutPolicy>
#include <MSeparator>

#include "../styles.h"

#undef DEBUG
#define WARNING
#include "../debug.h"

RightArrowItem::RightArrowItem (
        MBasicListItem::ItemStyle  style, 
        QGraphicsItem             *parent) : 
    MBasicListItem (style, parent),
    m_TitleStyleName ("CommonTitleInverted")
{
}

QGraphicsLayout *
RightArrowItem::createLayout()
{
    QGraphicsGridLayout *layout;
    MWidget             *spacer;
    MLabel              *titleLabel;
    MLabel              *subTitleLabel = 0;
    MImageWidget        *iconWidget    = 0;
    MImageWidget        *drillIconWidget;
    
    layout = new QGraphicsGridLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    titleLabel =  titleLabelWidget ();
    titleLabel->setWordWrap (true);

    drillIconWidget = new MImageWidget ("icon-m-common-drilldown-arrow-inverse");
    drillIconWidget->setStyleName ("CommonDrillDownIcon");

    titleLabel->setStyleName(m_TitleStyleName);

    switch (itemStyle()) {
        case TitleWithSubtitle:
            SYS_DEBUG ("TitleWithSubtitle");
            /*
             * The title label.
             */
            layout->addItem (titleLabel, 0, 0);
            layout->setAlignment(titleLabel, Qt::AlignLeft | Qt::AlignVCenter);
            /*
             * The sub-title label.
             */
            subTitleLabel = subtitleLabelWidget ();
            subTitleLabel->setStyleName("CommonSubTitleInverted");
            layout->addItem (subTitleLabel, 1, 0);
            layout->setAlignment (subTitleLabel, Qt::AlignLeft | Qt::AlignVCenter);

            spacer = new MWidget;
            layout->addItem (spacer, 2, 0);

            /*
             * The drill down icon.
             */
            layout->addItem(drillIconWidget, 0, 1, 3, 1);
            layout->setAlignment (drillIconWidget, Qt::AlignVCenter | Qt::AlignRight);
            break;

        case IconWithTitleAndSubtitle:
            SYS_DEBUG ("IconWithTitleAndSubtitle");
            /*
             * The left side icon.
             */
            iconWidget = imageWidget();
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
            subTitleLabel = subtitleLabelWidget ();
            subTitleLabel->setStyleName("CommonSubTitleInverted");
            layout->addItem (subTitleLabel, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
            /*
             * The drill down icon.
             */
            layout->addItem(drillIconWidget, 0, 2, 2, 1);
            layout->setAlignment (drillIconWidget, Qt::AlignVCenter | Qt::AlignRight);
            break;

        case IconWithTitle:
            SYS_DEBUG ("IconWithTitle");
            /*
             * The left side icon.
             */
            iconWidget = imageWidget();
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
RightArrowItem::setTitleStyleName (
        const QString &styleName)
{
    m_TitleStyleName = styleName;
}

/******************************************************************************
 *
 */
void
addSubTitle (
        QGraphicsWidget     *parent,
        MLinearLayoutPolicy *targetPolicy,
        const QString       &subTitle)
{
    MWidgetController       *container;
    QGraphicsLinearLayout   *layout;
    MLabel                  *label;
    MSeparator              *separator;

    container = new MWidgetController (parent);
    container->setContentsMargins (0., 0., 0., 0.);
    container->setStyleName (SUBTITLE_PANEL_STYLE_NAME);
    
    layout = new QGraphicsLinearLayout (Qt::Horizontal);
    layout->setContentsMargins (0., 0., 0., 0.);
    layout->setSpacing (0.);
    /*
     *
     */
    separator = new MSeparator;
    separator->setStyleName (SUBTITLE_DIVIDER_STYLE_NAME);

    /*
     *
     */
    label = new MLabel (subTitle);
    label->setStyleName (SUBTITLE_LABEL_STYLE_NAME);
    

    layout->addItem (separator);
    layout->setStretchFactor (separator, 2);

    layout->addItem (label);
    layout->setAlignment (label, Qt::AlignLeft);
    layout->setStretchFactor (label, 0);

    container->setLayout (layout);
    targetPolicy->addItem (container);
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



