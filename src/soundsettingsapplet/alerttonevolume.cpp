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
#include "alerttonevolume.h"

//#define DEBUG 
#define WARNING
#include "../debug.h"

#include <MWidgetCreator>
M_REGISTER_WIDGET_NO_CREATE(AlertToneVolume)

AlertToneVolume::AlertToneVolume(QGraphicsItem *parent):
	MSlider(parent),
	m_fileName(QString(ALERT_TONE_VOLUME_FILE_KEY)),
	m_volume(QString(ALERT_TONE_VOLUME_VOLUME_KEY)),
	m_preview(NULL)
{
	QList<QVariant> range;
	QProfileValue::RangeType rt;

    setStyleName ("CommonSliderInverted");

	range = m_volume.possibleValues(&rt);
	if (rt != QProfileValue::Interval)
		qWarning() << "AlertToneVolume::AlertToneVolume: volume key's range type is not \"interval\"";
	setRange(range[0].toInt(), range[1].toInt());
	setSteps(10);
    
	setValue(m_volume.value().toInt());
    setHandleLabel (m_volume.value().toString());

	connect(&m_volume, SIGNAL(changed()), 
            this, SLOT(volumeChanged()));
	connect(&m_fileName, SIGNAL(changed()), 
            this, SLOT(fileNameChanged()));
	connect(this, SIGNAL(valueChanged(int)), 
            this, SLOT(widgetVolumeChanged(int)));
	connect(this, SIGNAL(sliderPressed()), 
            this, SLOT(slotSliderPressed()));
	connect(this, SIGNAL(sliderReleased()), 
            this, SLOT(slotSliderReleased()));

}

AlertToneVolume::~AlertToneVolume()
{
	if (m_preview)
		delete m_preview;
}

void
AlertToneVolume::slotSliderPressed()
{
    SYS_DEBUG ("");
    setHandleLabelVisible (true);
	if (!m_preview)
		m_preview = new AlertTonePreview(m_fileName.value().toString());
}

void AlertToneVolume::slotSliderReleased()
{
    SYS_DEBUG ("");
    setHandleLabelVisible (false);
	if (m_preview) {
		delete m_preview;
		m_preview = NULL;
	}
}

void
AlertToneVolume::fileNameChanged()
{
	if (state() == MSliderModel::Pressed) {
		if (m_preview)
			delete m_preview;
		m_preview = new AlertTonePreview(m_fileName.value().toString());
	}
}

void
AlertToneVolume::volumeChanged()
{
//	if (state() == MSliderModel::Released)
		if (value() != m_volume.value().toInt())
			setValue(m_volume.value().toInt());
}

void
AlertToneVolume::widgetVolumeChanged(
        int newValue)
{
    setHandleLabel (QString::number(newValue));
    m_volume.set(QVariant(newValue));
}
