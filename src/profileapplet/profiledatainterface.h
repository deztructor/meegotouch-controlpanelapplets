/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#ifndef PROFILEDATAINTERFACE_H
#define PROFILEDATAINTERFACE_H

#include <QObject>
#include <QPair>

#ifdef HAVE_PROFILE_QT
class Profile;
#endif

class ProfileDataInterface : public QObject
{
    Q_OBJECT

public:
    
    /*
     * The order is fixed according to the UI specification.
     */
    enum ProfileId {
        ProfileIdSilent   = 0,
        ProfileIdBeep,
        ProfileIdRinging,
        ProfileIdLoud,
        // none must be last to teel how many profiles there are available
        ProfileIdNone 
    };

    struct ProfileData {
        ProfileId                 profileId;
        /* IconId, profilenam(translated) pair */
        QPair<QString, QString>   visualData;
        bool                      vibrationEnabled;
        bool                      isActive;
        int                       volumeLevel;
    };

    ProfileDataInterface ();
    virtual ~ProfileDataInterface ();

    static QString mapId (int id);

signals:
    void currentProfile (int id);
    void vibrationValue (int id, bool value);

public:
    QString getCurrentProfileIconId ();
    int getCurrentProfile ();
    QList<ProfileData> getProfilesData ();
    void setVibration (int id, bool value);

public slots:
    void setProfile (int value);

private slots:
    void currentProfileNameChanged (const QString &prof);

private:
    static ProfileId mapId (const QString &id);
    static QString id2Name (const QString &id);
    static QString mapId2IconID (int id);

private:
    #ifdef HAVE_PROFILE_QT
    Profile *m_ProfileAPI;
    #endif
};

#endif
