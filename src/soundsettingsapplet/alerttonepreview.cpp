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
#include "static.h"
#include "alerttonepreview.h"

//#define DEBUG
#define WARNING
#include "../debug.h"

AlertTonePreview::AlertTonePreview(const QString &fname):
	m_gstPipeline(NULL),
	m_gstFilesrc(NULL),
	m_gstVolume(NULL),
	m_profileVolume(QString(ALERT_TONE_VOLUME_VOLUME_KEY))
{
    SYS_DEBUG ("*** fname = %s", SYS_STR(fname));	
    SYS_WARNING ("Starting the playback.");

	GError *err = NULL;

	m_gstPipeline = gst_parse_launch(
		QString("filesrc name=alerttonepreviewfilesrc ! decodebin ! volume name=alerttonepreviewvolume ! autoaudiosink").toUtf8().constData(),
		&err);
	if (err) {
		qWarning() << "AlertTonePreview::AlertTonePreview:" << (err->message ? err->message : "Unknown error");
		g_error_free(err);
	}

	m_gstVolume = gst_bin_get_by_name(GST_BIN(m_gstPipeline), "alerttonepreviewvolume");
	m_gstFilesrc = gst_bin_get_by_name(GST_BIN(m_gstPipeline), "alerttonepreviewfilesrc");

	g_object_set(G_OBJECT(m_gstVolume), "volume", profileToGstVolume(), NULL);
	g_object_set(G_OBJECT(m_gstFilesrc), "location", fname.toUtf8().constData(), NULL);
	gst_bus_add_signal_watch(gst_element_get_bus(m_gstPipeline));
	g_signal_connect(G_OBJECT(gst_element_get_bus(m_gstPipeline)), "message", (GCallback)gstSignalHandler, this);
	gst_element_set_state(m_gstPipeline, GST_STATE_PLAYING);

	QObject::connect(&m_profileVolume, SIGNAL(changed()), this, SLOT(profileVolumeChanged()));
}

AlertTonePreview::~AlertTonePreview()
{
	SYS_WARNING ("Stopping the playback.");
	gst_element_set_state(m_gstPipeline, GST_STATE_NULL);
	gst_bus_remove_signal_watch(gst_element_get_bus(m_gstPipeline));
	gst_object_unref(m_gstPipeline);
}

void
AlertTonePreview::profileVolumeChanged()
{
	g_object_set(G_OBJECT(m_gstVolume), "volume", profileToGstVolume(), NULL);
}

double
AlertTonePreview::profileToGstVolume()
{
	QList<QVariant> range = m_profileVolume.possibleValues(NULL);
	return (((double)(m_profileVolume.value().toInt() - range[0].toInt())) / ((double)(range[1].toInt() - range[0].toInt())));
}

QString
AlertTonePreview::fname() const
{
	char *fname = NULL;

	g_object_get(G_OBJECT(m_gstFilesrc), "location", &fname, NULL);
	QString str(fname);
	g_free(fname);

    SYS_DEBUG ("returning %s", SYS_STR(str));
	return str;
}

void
AlertTonePreview::rewind()
{
    SYS_DEBUG ("");
	if (!gst_element_seek_simple(m_gstPipeline, GST_FORMAT_TIME, (GstSeekFlags)(GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT), 0l))
		qWarning() << "AlertTonePreview::rewind: seek failed";
}

void
AlertTonePreview::gstSignalHandler(GstBus *bus, GstMessage *msg, AlertTonePreview *atp)
{
	Q_UNUSED(bus)

	if (GST_MESSAGE_TYPE(msg) == GST_MESSAGE_ERROR) {
		GError *err = NULL;
		char *debug = NULL;

		gst_message_parse_error(msg, &err, &debug);
		qWarning() << "AlertTonePreview::gstSignalHandler:"
			<< "from" << GST_MESSAGE_SRC_NAME(msg)
			<< "type" << GST_MESSAGE_TYPE_NAME(msg)
			<< QString(err ? err->message ? err->message : "Unknown error" : "Unknown error")
			<< "debug:"
			<< QString(debug ? debug : "empty");

		g_error_free(err);
		g_free(debug);
	}
	else
	if (GST_MESSAGE_TYPE(msg) == GST_MESSAGE_EOS)
		atp->rewind();
}
