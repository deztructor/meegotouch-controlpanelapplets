/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "profiledatainterface.h"

/*
 * In the functional tests we use the real thing, in the unit tests we use the
 * stubbed version. 
 */
#if defined(UNIT_TEST) && !defined(FUNCTIONAL_TEST)
#  include "profilestub.h"
#else
#  include <Profile>
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
    return p1.volumeLevel < p2.volumeLevel;
}

/******************************************************************************
 * ProfileDataInterface class implementation.
 */
ProfileDataInterface::ProfileDataInterface ()
{
    m_ProfileAPI = new Profile ();

    SYS_DEBUG ("*** active profile = '%s'", 
            SYS_STR (m_ProfileAPI->activeProfile()));
    connect (m_ProfileAPI, SIGNAL(activeProfileChanged(QString)), 
            this, SLOT(currentProfileNameChanged(QString)));
}

ProfileDataInterface::~ProfileDataInterface ()
{
    delete m_ProfileAPI;
    m_ProfileAPI = NULL;
}


QString 
ProfileDataInterface::getCurrentProfileName ()
{
    SYS_DEBUG ("");
    QString prof = m_ProfileAPI->activeProfile();

    return id2Name (prof);
}

QString 
ProfileDataInterface::getCurrentProfileIconId ()
{
    SYS_DEBUG ("");

    return mapId2IconID (getCurrentProfile());
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

    QString prof = m_ProfileAPI->activeProfile();
    
    return mapId (prof);
}

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

        d.visualData.first = mapId2IconID (d.profileId);
        d.visualData.second = id2Name (id);
        d.vibrationEnabled  = m_ProfileAPI->isVibrationEnabled (id);
        d.volumeLevel       = m_ProfileAPI->volumeLevel (id);
        data.append(d);
    }

    /*
     * We return the profiles in the order of the loudness. Please check
     * NB#188710 for further details.
     */
    qSort (data.begin(), data.end(), ProfileDataLessThan);
    #if 0
    SYS_DEBUG ("data.count () = %d", data.count ());
    for (int i = 0; i < data.size(); ++i) {
        SYS_DEBUG ("************* profile %d ***********", i);
        SYS_DEBUG ("*** visualData.second = %s",
                SYS_STR(data[i].visualData.second));
        SYS_DEBUG ("*** volumeLevel       = %d",
                data[i].volumeLevel);
    }
    #endif
    return data;
}

void 
ProfileDataInterface::setProfile (
        int value)
{
    SYS_DEBUG ("value = %d", value);

    bool success = m_ProfileAPI->setActiveProfile (mapId(value));
    if (!success) {
        SYS_WARNING ("Failed setting profile.");
        // TODO: what??
    }
}

void 
ProfileDataInterface::setVibration (
        int    id, 
        bool   value)
{
    SYS_DEBUG ("id = %d, value = %s", id, SYS_BOOL (value));

    bool success = m_ProfileAPI->setVibration(mapId(id), value);
    if (!success) {
        SYS_WARNING ("Failed setting the vibration.");
        // TODO: what??
    }
}

QString ProfileDataInterface::mapId2IconID (int id)
{
    QString iconId = "";

    switch (id) {
        case ProfileIdRinging:
            iconId = QString("icon-m-common-volume");
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


