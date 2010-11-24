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
#include <gst/gst.h>
#include <MAction>

#include "soundsettingsapplet.h"
#include "alerttone.h"
#include "alerttonetoplevel.h"
#include "alerttonebrowser.h"
#include "alerttoneappletwidget.h"

#define WARNING
#include "../debug.h"

#ifndef UNIT_TEST
#include <MLibrary>
M_LIBRARY
#endif

Q_EXPORT_PLUGIN2(soundsettingsapplet, SoundSettingsApplet)

SoundSettingsApplet::SoundSettingsApplet()
{
}

SoundSettingsApplet::~SoundSettingsApplet()
{
	gst_deinit ();
}

static int    gst_argc = 1;
static char*  gst_argv[] = { (char*) "arg", 0 };

void
SoundSettingsApplet::init()
{
	gst_init (&gst_argc, (char ***) &gst_argv);

	m_alertTones = AlertTone::alertTones();
}

/* widgetId: 0xaaaabbbb where 
   0xaaaa is the widget ID and 
	 0xbbbb is the alert tone index */
DcpWidget *
SoundSettingsApplet::constructWidget(int widgetId)
{
	AlertToneToplevel *newWidget = NULL;
	int realWidgetId = widgetId / 65536;
	int alertToneIdx = widgetId - realWidgetId * 65536;

	if (m_stack.size() > 0)
		if (((m_stack.top()->getWidgetId() / 65536) == AlertToneBrowser_id      && realWidgetId == AlertToneBrowser_id) ||
		    ((m_stack.top()->getWidgetId() / 65536) == AlertToneAppletWidget_id && realWidgetId != AlertToneBrowser_id))
			return NULL;

	if (AlertToneAppletWidget_id == realWidgetId)
		newWidget = new AlertToneAppletWidget(m_alertTones);
	else
	if (AlertToneBrowser_id == realWidgetId && alertToneIdx >= 0 && alertToneIdx < m_alertTones.size())
		newWidget = new AlertToneBrowser(m_alertTones[alertToneIdx]);
	else
		SYS_WARNING ("Invalid widgetId = %d", widgetId);

	if (newWidget) {
		m_stack.push(newWidget);
		QObject::connect(newWidget, SIGNAL(destroyed(QObject *)), this, SLOT(toplevelDestroyed(QObject *)));
	}
	return newWidget;
}

void
SoundSettingsApplet::toplevelDestroyed(QObject *goner)
{
	if (m_stack.size() > 0)
		if (goner == qobject_cast<QObject *>(m_stack.top()))
			m_stack.pop();
}

QString
SoundSettingsApplet::title() const
{
	QString title = qtTrId("qtn_sond_sounds");

	if (m_stack.size() > 0)
  	if (m_stack.top())
			title = qobject_cast<AlertToneToplevel *>(m_stack.top())->title();

	return title;
}

QVector<MAction*> 
SoundSettingsApplet::viewMenuItems()
{
	QVector<MAction*> vector;

	if (m_stack.size() > 0)
		if (m_stack.top())
			vector = qobject_cast<AlertToneToplevel *>(m_stack.top())->viewMenuItems();

	return vector;
}

DcpBrief *
SoundSettingsApplet::constructBrief (
        int partId)
{
    Q_UNUSED (partId);
    return NULL;
}

