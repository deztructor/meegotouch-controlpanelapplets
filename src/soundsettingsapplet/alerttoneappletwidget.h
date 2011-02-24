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

#ifndef _ALERT_TONE_APPLET_WIDGET_H_
#define _ALERT_TONE_APPLET_WIDGET_H_

#include <MContainer>
#include "alerttonetoplevel.h"
#include "alerttone.h"

class MLabel;

class AlertToneAppletWidget : public AlertToneToplevel
{
	Q_OBJECT

public:
	AlertToneAppletWidget(QList<AlertTone *>alertTones, QGraphicsWidget *parent = 0);

private:
	virtual void retranslateUi();
	MContainer *createAlertTonesList(QGraphicsWidget *parent);
	MContainer *createFeedbackList(QGraphicsWidget *parent);

	QList<AlertTone *>  m_alertTones;
	MContainer         *m_tones;
	MContainer         *m_feedback;

    MLabel             *m_Title;
    MLabel             *m_EventTonesLabel;
    MLabel             *m_FeedbackLabel;

protected:
    virtual void polishEvent ();
	virtual void createContents();

#ifdef UNIT_TEST
    friend class Ut_AlertToneAppletWidgetTests;
#endif
};

#endif /* !_ALERT_TONE_APPLET_WIDGET_H_ */
