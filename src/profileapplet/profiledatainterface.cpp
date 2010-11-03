/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of meegotouch-controlpanelapplets.
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

#include "profiledatainterface.h"

/*
 * In the functional tests we use the real thing, in the unit tests we use the
 * stubbed version.
 *
 * FIXME: This should be eliminated, the code should not be different for
 * testing.
 */
#ifdef HAVE_PROFILE_QT
#  if defined(UNIT_TEST) && !defined(FUNCTIONAL_TEST)
#    include "profilestub.h"
#  else
#    include <Profile>
#  endif
#else
  namespace ProfileName {
    const QString ringing = "general";  /*!< Profile name 'ringing' */
    const QString silent = "silent";    /*!< Profile name 'silent' */
    const QString beep = "meeting";        /*!< Profile name 'beep' */
    const QString loud = "outdoors";        /*!< Profile name 'loud' */
  }
#endif

#include <QStringList>
#include <QList>

#undef DEBUG
#include "../debug.h"

using namespace ProfileName;


/******************************************************************************
 * ProfileData support methods implementation.
 */
bool ProfileDataLessThan (
        const ProfileDataInterface::ProfileData &p1,
        const ProfileDataInterface::ProfileData &p2)
{
    // Use this to sort the profiles in volume level order
    //return p1.volumeLevel < p2.volumeLevel;
    return p1.profileId < p2.profileId;
}

/******************************************************************************
 * ProfileDataInterface class implementation.
 */
ProfileDataInterface::ProfileDataInterface ()
{
    #ifdef HAVE_PROFILE_QT
    m_ProfileAPI = new Profile ();
    connect (m_ProfileAPI, SIGNAL(activeProfileChanged(QString)), 
            this, SLOT(currentProfileNameChanged(QString)));
    #endif
}

ProfileDataInterface::~ProfileDataInterface ()
{
    #ifdef HAVE_PROFILE_QT
    delete m_ProfileAPI;
    m_ProfileAPI = NULL;
    #endif
}


/*!
 * returns the status-menu icon ID representing the current profile.
 */
QString
ProfileDataInterface::mapId2StatusIconId (int id)
{
    QString iconId;

    switch (id) {
        case ProfileIdRinging:
            iconId = QString ("icon-m-status-menu-normal");
            break;

        case ProfileIdSilent:
            iconId = QString ("icon-m-status-menu-profile-silent");
            break;

        case ProfileIdBeep:
            iconId = QString ("icon-m-status-menu-profile-beep");
            break;

        case ProfileIdLoud:
            iconId = QString ("icon-m-status-menu-profile-loud");
    }

    return iconId;
}

void 
ProfileDataInterface::currentProfileNameChanged (
        const QString &prof)
{
    emit currentProfile(mapId(prof));
}

int 
ProfileDataInterface::getCurrentProfile ()
{
    SYS_DEBUG ("");
    
    #ifdef HAVE_PROFILE_QT
    QString prof = m_ProfileAPI->activeProfile();
    return mapId (prof);
    #else
    /*
     * FIXME: To implement the code that works without the ProfileQt library.
     */
    SYS_WARNING ("Not implemented.");
    return 0;
    #endif
}


#ifdef HAVE_PROFILE_QT
QList<ProfileDataInterface::ProfileData> 
ProfileDataInterface::getProfilesData ()
{
    SYS_DEBUG ("");
    QList<ProfileData> data;

    // send profile <name, is> map
    QStringList ids = m_ProfileAPI->profileNames ();

    // send...
    for (int i = 0; i < ids.count(); ++i) {
        ProfileDataInterface::ProfileData d;
        //get name...
        QString id = ids.at(i);

        SYS_DEBUG ("profile-id : '%s'", SYS_STR (id));
        d.profileId = mapId (id);
        if (d.profileId == ProfileIdNone)
            continue;

        d.profileName       = id2Name (id);
        d.vibrationEnabled  = m_ProfileAPI->isVibrationEnabled (id);
        d.volumeLevel       = m_ProfileAPI->volumeLevel (id);
        data.append(d);
    }

    /*
     * We return the profiles in a specific order. Please check NB#188710 for
     * further details.
     */
    qSort (data.begin(), data.end(), ProfileDataLessThan);
    return data;
}
#else
QList<ProfileDataInterface::ProfileData> 
ProfileDataInterface::getProfilesData ()
{
    QList<ProfileData> data;

    /*
     * FIXME: To implement a variant that does not depend on the ProfileQt
     * library.
     */
    SYS_WARNING ("Unimplemented.");

    return data;
}
#endif

void 
ProfileDataInterface::setProfile (
        int value)
{
    bool success;

    SYS_DEBUG ("value = %d", value);

    #ifdef HAVE_PROFILE_QT
    success = m_ProfileAPI->setActiveProfile (mapId(value));
    if (!success) {
        SYS_WARNING ("Failed setting profile.");
    }
    #else
    /*
     * FIXME: To implement a codepiece that sets the current profile without the
     * help of the ProfileQt library.
     */
    SYS_WARNING ("Not implemented.");
    #endif
}

void 
ProfileDataInterface::setVibration (
        int    id, 
        bool   value)
{
    bool success;

    SYS_DEBUG ("id = %d, value = %s", id, SYS_BOOL (value));

    #ifdef HAVE_PROFILE_QT
    success = m_ProfileAPI->setVibration(mapId(id), value);
    if (!success) {
        SYS_WARNING ("Failed setting the vibration.");
    }
    #else
    /*
     * FIXME: To implement a codepiece that sets the vibration for a specific
     * profile without the help of the ProfileQt library.
     */
    SYS_WARNING ("Not implemented.");
    #endif
}

/*!
 * returns The icon ID representing the given profile.
 */
QString 
ProfileDataInterface::mapId2IconId (int id)
{
    QString iconId = "";

    switch (id) {
        case ProfileIdRinging:
            iconId = QString("icon-m-profile-normal");
            break;
        case ProfileIdSilent:
            iconId = QString("icon-m-profile-silent");
            break;
        case ProfileIdBeep:
            iconId = QString("icon-m-profile-beep");
            break;
        case ProfileIdLoud:
            iconId = QString("icon-m-profile-loud");
            break;
    }

    return iconId;
}
QString 
ProfileDataInterface::id2Name (
        const QString &id)
{
    QString localised = "";
    if (ProfileName::ringing == id) {
        //% "Ringing"
        localised = qtTrId ("qtn_prof_ringing");
    } else if (ProfileName::silent == id) {
        //% "Silent"
        localised = qtTrId ("qtn_prof_silent");
    } else if (ProfileName::beep == id) {
        //% "Beep"
        localised = qtTrId ("qtn_prof_beep");
    } else if (ProfileName::loud == id) {
        //% "Loud"
        localised = qtTrId ("qtn_prof_loud");
    }

    SYS_DEBUG ("returning '%s'", SYS_STR (localised));
    return localised;
}

ProfileDataInterface::ProfileId 
ProfileDataInterface::mapId (
        const QString &id)
{
    ProfileId intId = ProfileIdNone;

    if (ProfileName::ringing == id) {
        intId = ProfileIdRinging;
    } else if (ProfileName::silent == id) {
        intId = ProfileIdSilent;
    } else if (ProfileName::beep == id) {
        intId = ProfileIdBeep;
    } else if (ProfileName::loud == id) {
        intId = ProfileIdLoud;
    }

    return intId;
}

QString 
ProfileDataInterface::mapId (
        int id)
{
    QString stringId = "";

    switch (id) {
        case ProfileIdRinging:
            stringId = ProfileName::ringing;
            break;
        case ProfileIdSilent:
            stringId = ProfileName::silent;
            break;
        case ProfileIdBeep:
            stringId = ProfileName::beep;
            break;
        case ProfileIdLoud:
            stringId = ProfileName::loud;
            break;
    }

    return stringId;
}


