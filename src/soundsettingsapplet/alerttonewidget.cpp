/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

#include <QDebug>
#include <MImageWidget>
#include <MLabel>
#include <QGraphicsGridLayout>

#include "alerttonewidget.h"
#include "alerttoneappletmaps.h"
#include "soundsettingsapplet.h"

AlertToneWidget::AlertToneWidget (
        AlertTone           *tone, 
        int                  idx, 
        AlertToneToplevel   *changer, 
        QGraphicsItem       *parent) :
    DrillDownItem (MBasicListItem::TitleWithSubtitle, parent),
	m_tone(tone),
	m_changer(changer),
	m_idx(idx)
{
    /*
     *
     */
	connect (m_tone, SIGNAL(changed()), 
            this, SLOT(alertToneChanged()));
	connect (m_tone, SIGNAL(refreshed()), 
            this, SLOT(alertToneChanged()));
	connect(this, SIGNAL(clicked()), 
            this, SLOT(clicked()));
	
    setProperty ("title", AlertToneAppletMaps::mapToUiString(m_tone->key()));
	setProperty ("subtitle", QVariant(m_tone->niceName()));
}

#if 0
QGraphicsLayout *
AlertToneWidget::createLayout()
{
    QGraphicsGridLayout *layout;
    MLabel              *titleLabel;
    MLabel              *subTitleLabel;
    MImageWidget        *iconWidget;
    
    layout = new QGraphicsGridLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    
    titleLabel =  titleLabelWidget ();
    subTitleLabel = subtitleLabelWidget ();
    iconWidget = imageWidget();

    /*
     * The title label.
     */
    titleLabel->setStyleName("CommonTitleInverted");
    layout->addItem (titleLabel, 0, 0, Qt::AlignLeft | Qt::AlignVCenter);

    /*
     * The sub-title label.
     */
    subTitleLabel->setStyleName("CommonSubTitleInverted");
    layout->addItem (subTitleLabel, 1, 0, Qt::AlignLeft | Qt::AlignVCenter);

    /*
     * The drill down icon.
     */
    iconWidget->setImage("icon-m-common-drilldown-arrow");
    // FIXME: According to the layout guide we should use CommonDrilldownIcon,
    // but that does not look good. It makes the icon pixelated.
    iconWidget->setStyleName("CommonMainIcon");
    layout->addItem(iconWidget, 0, 1, 2, 1);
    layout->setAlignment (iconWidget, Qt::AlignVCenter | Qt::AlignRight);

    setStyleName ("CommonPanelInverted");

    return layout;
}
#endif

void
AlertToneWidget::alertToneChanged()
{
	setProperty("subtitle", QVariant(m_tone->niceName()));
}

void
AlertToneWidget::clicked()
{
	m_changer->emit_changeWidget((SoundSettingsApplet::AlertToneBrowser_id * 65536) + m_idx);
}

void
AlertToneWidget::retranslateUi()
{
	setProperty ("title", AlertToneAppletMaps::mapToUiString(m_tone->key()));
}
