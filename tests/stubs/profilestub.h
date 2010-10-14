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

#ifndef __PROFILESTUB_H__
#define __PROFILESTUB_H__

#include <QObject>
#include <QString>
#include <QStringList>

namespace ProfileName
{
const QString ringing = "general";  /*!< Profile name 'ringing' */
const QString silent = "silent";    /*!< Profile name 'silent' */
const QString beep = "meeting";        /*!< Profile name 'beep' */
const QString loud = "outdoors";        /*!< Profile name 'loud' */
}

class Profile : public QObject 
{
Q_OBJECT

public:
    Profile ();
    ~Profile ();

public slots:
    QString activeProfile ();
    bool setActiveProfile(QString name);
    QStringList profileNames ();
    bool isVibrationEnabled (QString profileName);
    int volumeLevel (QString name);
    bool setVibration (
            QString  profileName, 
            bool     enabled);

private:
    int nameToId (QString name);

    QString m_ActiveProfileName;
    bool    m_Vibration[4];
};

#endif

