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

#ifndef _ALERT_TONE_VOLUME_H_
#define _ALERT_TONE_VOLUME_H_

#include <MSlider>
#include "qprofilevalue.h"
#include "alerttonepreview.h"

class AlertToneVolume : public MSlider
{
	Q_OBJECT

public:
	AlertToneVolume(QGraphicsItem *parent = 0);
	~AlertToneVolume();

private:
	QProfileValue m_fileName;
	QProfileValue m_volume;
	AlertTonePreview *m_preview;

private slots:
	void volumeChanged();
	void fileNameChanged();
	void slotSliderPressed();
	void slotSliderReleased();
	void widgetVolumeChanged(int newValue);
#ifdef UNIT_TEST
    friend class Ut_AlertToneVolumeTests;
#endif
};

#endif /* !_ALERT_TONE_VOLUME_H_ */
