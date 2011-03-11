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
#include "alerttone.h"
#include "trackerconnection.h"

#include <QtTracker/Tracker>
#ifdef HAVE_LIBPROFILE
#include <profiled/libprofile.h>
#endif

//#define DEBUG
#define WARNING
#include "../debug.h"

/*!
 * The constructor of the class sets the key that can't be changed later.
 */
AlertTone::AlertTone(
        const QString &key):
	QProfileValue(key, true)
{
    SYS_DEBUG ("*** key = %s", SYS_STR(key));
    /*
     * TrackerConnection might send a signal about the tracker answer when it is
     * available.
     */
    connect (TrackerConnection::instance(), 
            SIGNAL(dataReady(QString,QString,QString)),
            this, 
            SLOT(dataReceived(QString,QString,QString)));
}

/*!
 * A method for getting AlertTone objects for all
 * available settings (ringing, voip, email, sms, im)
 *
 * \return AlertTone instances...
 */
QList<AlertTone *>
AlertTone::alertTones()
{
	QList<AlertTone *> v;

#ifdef HAVE_LIBPROFILE
    QList<QString> keys;
    	char **profiles = profile_get_profiles();
	for (int Nix = 0 ; profiles[Nix] != NULL ; Nix++)
	    SYS_DEBUG("profiles[%d] = %s\n", Nix, SYS_STR(profiles[Nix]));
	profile_free_profiles(profiles);
    
	profileval_t *vals = profile_get_values(NULL);

	if (vals)
		for (int Nix = 0 ; vals[Nix].pv_key != NULL ; Nix++) {
			QStringList split_key = QString(vals[Nix].pv_key).split('.');

			if (split_key.size() == 3) {
				if (split_key[0] != "clock" && split_key[1] == "alert" && split_key[2] == "tone")
						keys.push_back(vals[Nix].pv_key);
					
			}
		}

	QList<QString> list;
	list << "ringing.alert.tone"
		  << "voip.alert.tone"
		  << "email.alert.tone"
		  << "sms.alert.tone"
		  << "im.alert.tone"
		  << "calendar.alert.tone";

	for (int Nix = 0 ; Nix < list.size() ; Nix++)
	{
		if  (keys.contains(list.at(Nix)))
	    {
			keys.removeOne(list.at(Nix));
			v << new AlertTone(QString(list.at(Nix)) + "@general");
	    }
	 }	 

	for (int Nix = 0 ; Nix < keys.size() ; Nix++ )
	v << new AlertTone(QString(keys.at(Nix)));

	profile_free_values(vals);
#endif

	return v;
}

/*!
 * \returns The human readable name of the sound file.
 *
 * For the sound files under the /home directory this method will return the
 * sound file title provided by the tracker subsystem by reading the sound file
 * itself. For files under a different diretory the method will create the nice
 * name from the filename, because tracker will not provide file information for
 * these files. In this case the nice name is created from the basename of the
 * file by removing the file extension and changing underscore ('-') characters
 * to spaces (' ').
 */
QString
AlertTone::niceName()
{
	maybeUpdate();
	return m_niceName;
}

/*!
 * \returns the tracker-id of the sound file when it is available.
 */
QString
AlertTone::trackerId()
{
	maybeUpdate();
	return m_trackerId;
}

void
AlertTone::maybeUpdate()
{
	if (m_val.isNull()) {
        SYS_DEBUG ("m_val is NULL...");
		m_niceName =  "";
		m_trackerId = "";
		fetchFromBackend();
	} else if (m_niceName.isEmpty()) {
        m_niceName = TrackerConnection::instance()->niceNameFromFileName (
            m_val.toString());
    }
}

void
AlertTone::realSetValue(
        const QVariant &newValue)
{
	if (m_val != newValue) {
		m_niceName.clear();
		m_trackerId.clear();
		QProfileValue::realSetValue(newValue);
	}
}

void
AlertTone::fetchFromBackend()
{
	QProfileValue::fetchFromBackend();
    m_niceName = TrackerConnection::instance()->niceNameFromFileName (
            m_val.toString());
}

/*!
 * \returns The full path of the currently set sound file.
 */
QString 
AlertTone::fileName()
{
	QProfileValue::fetchFromBackend();

	if (!m_val.isNull())
	    return m_val.toString();

    return QString("");
}

void
AlertTone::dataReceived (
            const QString   &filename, 
            const QString   &title,
            const QString   &trackerId)
{
    if (!m_val.isNull() && m_val.toString() == filename) {
        SYS_DEBUG ("Got an answer...");
        m_niceName = title;
        m_trackerId = trackerId;
        emit refreshed();
    }
}
