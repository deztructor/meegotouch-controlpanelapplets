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

#ifndef _SOUND_SETTINGS_APPLET_H_
#define _SOUND_SETTINGS_APPLET_H_

#include <MAction>
#include <QStack>
#include "dcpappletif.h"
#include "alerttone.h"
#include "alerttonetoplevel.h"

class SoundSettingsApplet : public QObject, public DcpAppletIf
{
	Q_OBJECT
	Q_INTERFACES(DcpAppletIf)

public:
	SoundSettingsApplet();
	~SoundSettingsApplet();

	static const int AlertToneAppletWidget_id = 0;
	static const int AlertToneBrowser_id = 1;

	virtual void init();
	virtual DcpWidget* constructWidget(int widgetId);
	virtual QString title() const;
	virtual QVector<MAction *> viewMenuItems();
	virtual DcpBrief* constructBrief(int partId);

#ifndef UNIT_TEST
private:
#endif /* !UNIT_TEST */
	QStack<AlertToneToplevel *>m_stack;
	int m_argc;
	char **m_argv;
	QList<AlertTone *> m_alertTones;

#ifdef UNIT_TEST
public slots:
#else
private slots:
#endif /* !UNIT_TEST */
	void toplevelDestroyed(QObject *goner);
};

#endif /* !_SOUND_SETTINGS_APPLET_H_ */
