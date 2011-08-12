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

#include <QDebug>
#include <QVariant>
#include "alerttoneappletmaps.h"

//#define DEBUG
#define WARNING
#include "../debug.h"

const AlertToneAppletMaps AlertToneAppletMaps::maps;

const AlertToneAppletMaps &
AlertToneAppletMaps::instance() { return maps; }
AlertToneAppletMaps::AlertToneAppletMaps()
{
	/* Alert tone MContentItem widget titles */
	m_map["ringing.alert.tone@general"]                      = "qtn_sond_ring_tone";
	m_map["voip.alert.tone@general"]                         = "qtn_sond_internet_tone";
	m_map["email.alert.tone@general"]                        = "qtn_sond_email_tone";
	m_map["sms.alert.tone@general"]                          = "qtn_sond_message_tone";
	m_map["im.alert.tone@general"]                           = "qtn_sond_ins_messaging";
	m_map["calendar.alert.tone@general"]                     = "qtn_sond_org_reminders";

	/* Combo box widget titles */
	m_map["keypad.sound.level"]                      = "qtn_sond_keyboard";
	m_map["system.sound.level"]                      = "qtn_sond_system";
	m_map["/meegotouch/input_feedback/volume/priority2/pulse"] = "qtn_sond_touch_screen";
	m_map["/meegotouch/input_feedback/volume/priority2/vibra"] = "qtn_sond_touch_vibra";

	/* GConf combo box values */
	m_map["off"]                                     = "qtn_comm_settings_off";
	m_map["low"]                                     = "qtn_sond_level_1";
	m_map["medium"]                                  = "qtn_sond_level_2";
	m_map["high"]                                    = "qtn_sond_level_3";

	/* Profile combo box values */
	m_map["0"]                                       = "qtn_comm_settings_off";
	m_map["1"]                                       = "qtn_sond_level_1";
	m_map["2"]                                       = "qtn_sond_level_2";
	m_map["3"]                                       = "qtn_sond_level_3";

    /* XXX: Please keep this for eng.english generation: */
#if 0
    //% "Off"
    qtTrId ("qtn_comm_settings_off");
    //% "Level 1"
    qtTrId ("qtn_sond_level_1");
    //% "Level 2"
    qtTrId ("qtn_sond_level_2");
    //% "Level 3"
    qtTrId ("qtn_sond_level_3");
    //% "System sounds"
    qtTrId ("qtn_sond_system");
    //% "Keyboard feedback"
    qtTrId ("qtn_sond_keyboard");
    //% "Touch screen tones"
    qtTrId ("qtn_sond_touch_screen");
    //% "Touch screen vibration"
    qtTrId ("qtn_sond_touch_vibra");
    //% "Ringing tone"
    qtTrId ("qtn_sond_ring_tone");
    //% "Internet tone"
    qtTrId ("qtn_sond_internet_tone");
    //% "E-mail tone"
    qtTrId ("qtn_sond_email_tone");
    //% "SMS tone"
    qtTrId ("qtn_sond_message_tone");
    //% "Instant messaging tone"
    qtTrId ("qtn_sond_ins_messaging");
    //% "Calendar reminder tone"
	qtTrId ("qtn_sond_org_reminders");
#endif
}

QString
AlertToneAppletMaps::map(const QString &str) const
{
	return (m_map.contains(str)) ? m_map[str] : str;
}

QString
AlertToneAppletMaps::map(const int &i) const
{
	return (m_map.contains(QVariant(i).toString())) ? 
        m_map[QVariant(i).toString()] : QVariant(i).toString();
}

/*!
 * Takes a GConf stored string as an abstract representation and translates it
 * to a user interface string. Returns the UI string as a QVariant for
 * it is convenient to store in models.
 */
QVariant
AlertToneAppletMaps::mapToUiString (
        const QString &str) 
{
    const AlertToneAppletMaps self = AlertToneAppletMaps::instance();
    const QString mapped = self.map (str);
    const QString translated = qtTrId (mapped.toUtf8().constData());

    SYS_DEBUG ("%s -> %s", SYS_STR(str), SYS_STR(translated));
    return QVariant (translated);
}

/*
 * Overloaded version that accepts integer for paramater.
 */
QVariant
AlertToneAppletMaps::mapToUiString (
        int integer)
{
    const AlertToneAppletMaps self = AlertToneAppletMaps::instance();
    const QString mapped = self.map (integer);
    const QString translated = qtTrId (mapped.toUtf8().constData());

    SYS_DEBUG ("%d -> %s", integer, SYS_STR(translated));
    return QVariant (translated);
}
