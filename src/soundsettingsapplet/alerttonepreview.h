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

#ifndef _ALERT_TONE_PREVIEW_H_
#define _ALERT_TONE_PREVIEW_H_

#include <gst/gst.h>
#include <QObject>
#include <QString>
#include "qprofilevalue.h"

#if HAVE_LIBRESOURCEQT
#include <policy/resource-set.h>
#endif

class AlertTonePreview : public QObject
{
	Q_OBJECT

public:
	AlertTonePreview(const QString &fname);
	~AlertTonePreview();
	QString fname() const;

private:
	GstElement *m_gstPipeline;
	GstElement *m_gstFilesrc;
	GstElement *m_gstVolume;
	QProfileValue m_profileVolume;
        QString     m_Filename;

        void gstInit();
	void rewind();
	double profileToGstVolume();

	static void gstSignalHandler(GstBus *bus, GstMessage *msg, AlertTonePreview *atp);

#if (HAVE_LIBRESOURCEQT)
    void        getResources();
    ResourcePolicy::ResourceSet *resources;
#endif

private slots:
	void profileVolumeChanged();
#if (HAVE_LIBRESOURCEQT)
    //! An internal slot to handle the case when we got the hardware volume keys resource
    void audioResourceAcquired();
    //! An internal slot to handle the case when we lost the hardware volume keys resource
    void audiResourceLost();
#endif

#ifdef UNIT_TEST
    friend class Ut_AlertTonePreviewTests;
#endif
};

#endif /* !_ALERT_TONE_PREVIEW_H_ */
