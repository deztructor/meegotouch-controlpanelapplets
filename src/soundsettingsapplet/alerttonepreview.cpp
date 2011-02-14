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
#include <QApplication>

//#define DEBUG
#define WARNING
#include "../debug.h"

static const char *GstStartCommand = 
    "filesrc name=alerttonepreviewfilesrc ! "
"decodebin ! volume name=alerttonepreviewvolume ! pulsesink name=alerttonepreviewpulsesink";

AlertTonePreview::AlertTonePreview(const QString &fname):
       m_gstPipeline(NULL),
       m_gstFilesrc(NULL),
       m_gstVolume(NULL),
        m_profileVolume(QString(ALERT_TONE_VOLUME_VOLUME_KEY)),
        m_Filename(fname)
{
#ifdef HAVE_LIBRESOURCEQT
    getResources();
#else
    gstInit();
#endif
}

AlertTonePreview::~AlertTonePreview()
{
    SYS_DEBUG ("Stopping the playback.");
    gst_element_set_state(m_gstPipeline, GST_STATE_NULL);
    gst_bus_remove_signal_watch(gst_element_get_bus(m_gstPipeline));
    gst_object_unref(m_gstPipeline);
#ifdef HAVE_LIBRESOURCEQT
    resources->release ();
#endif
}

void
AlertTonePreview::gstInit()
{
    GError *err = NULL;
    SYS_DEBUG ("*** fname = %s", SYS_STR(m_Filename));
    SYS_DEBUG ("Starting the playback.");
    GstElement *pulsesink;
    GstStructure *props;

    m_gstPipeline = gst_parse_launch (GstStartCommand, &err);
    if (err) {
            SYS_WARNING ("ERROR from gst_parse_launch: %s",
            err->message ? err->message : "Unknown error");
            g_error_free(err);
    goto finalize;
    }

    m_gstVolume = gst_bin_get_by_name(
        GST_BIN(m_gstPipeline), "alerttonepreviewvolume");
    m_gstFilesrc = gst_bin_get_by_name(
        GST_BIN(m_gstPipeline), "alerttonepreviewfilesrc");
    pulsesink = gst_bin_get_by_name(
        GST_BIN(m_gstPipeline), "alerttonepreviewpulsesink");

    if (m_gstVolume && m_gstFilesrc) {
        g_object_set (G_OBJECT(m_gstVolume),
                "volume", profileToGstVolume(),
                NULL);
        g_object_set (G_OBJECT(m_gstFilesrc),
                "location", m_Filename.toUtf8().constData(),
                NULL);
    }

    props = gst_structure_from_string ("props,media.role=AlertTonePreview", NULL);
    if(pulsesink && props)
        g_object_set (G_OBJECT(pulsesink),
                      "stream-properties", props,
                      NULL);
    else
        SYS_DEBUG("Cannot set media.role property!");
    gst_structure_free(props);

    gst_bus_add_signal_watch(gst_element_get_bus(m_gstPipeline));
    g_signal_connect(G_OBJECT(gst_element_get_bus(m_gstPipeline)),
        "message", (GCallback)gstSignalHandler, this);
    gst_element_set_state(m_gstPipeline, GST_STATE_PLAYING);

finalize:
        connect(&m_profileVolume, SIGNAL(changed()),
            this, SLOT(profileVolumeChanged()));
}

void
AlertTonePreview::getResources()
{
#ifdef HAVE_LIBRESOURCEQT
    resources = new ResourcePolicy::ResourceSet("player");
    resources->addResource(ResourcePolicy::AudioPlaybackType);

    ResourcePolicy::AudioResource *audioResource =
            new ResourcePolicy::AudioResource("player");

    audioResource->setProcessID(QApplication::applicationPid());
    /*
     * This is for the libresource to be able to identify our pulseaudio stream,
     * so this is very important that the same key-value pair should be set
     * for the pulsesink. The value should be uniqe.
     */
    audioResource->setStreamTag("media.role", "AlertTonePreview");

    resources->addResourceObject(audioResource);
    connect(resources, SIGNAL(resourcesGranted(QList<ResourcePolicy::ResourceType>)),
            this, SLOT(audioResourceAcquired()));
    connect(resources, SIGNAL(lostResources()), this, SLOT(audiResourceLost()));
    connect(resources, SIGNAL(resourcesBecameAvailable (const QList< ResourcePolicy::ResourceType >)),
            this, SLOT(audioResourcesBecameAvailable()));
    resources->acquire();
#endif
}


void
AlertTonePreview::audioResourceAcquired()
{
    SYS_DEBUG("");
    gstInit();
}

void
AlertTonePreview::audiResourceLost()
{
    SYS_DEBUG("");
    gst_element_set_state(m_gstPipeline, GST_STATE_PAUSED);
}

void
AlertTonePreview::audioResourcesBecameAvailable()
{
    SYS_DEBUG("");
    gst_element_set_state(m_gstPipeline, GST_STATE_PLAYING);
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
    return m_Filename;
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
