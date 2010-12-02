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
