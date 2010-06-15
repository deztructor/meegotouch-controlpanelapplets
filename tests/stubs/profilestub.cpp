/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of systemui.
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
#include "profilestub.h"

#define DEBUG
#include "../../src/debug.h"

Profile::Profile ()
{
    SYS_DEBUG ("Constructor: %p", this);
    m_ActiveProfileName = ProfileName::ringing; 
}

Profile::~Profile ()
{
    SYS_DEBUG ("Destructor : %p", this);
}

QString 
Profile::activeProfile ()
{
    return m_ActiveProfileName;
}

bool 
Profile::setActiveProfile (
        QString name)
{
    m_ActiveProfileName = name;
    return true;
}


QStringList
Profile::profileNames ()
{
    QStringList retval;

    retval << "general" << "silent" << "meeting" << "outdoors";

    return retval;
}
   
bool
Profile::isVibrationEnabled (
        QString profileName)
{
    return m_Vibration[nameToId(profileName)];
}

bool
Profile::setVibration (
            QString  profileName, 
            bool     enabled)
{
    m_Vibration[nameToId(profileName)] = enabled;
    return true;
}

int 
Profile::nameToId (
        QString name)
{
    if (name == ProfileName::ringing)
        return 0;
    else if (name == ProfileName::silent)
        return 1;
    else if (name == ProfileName::beep)
        return 2;
    else if (name == ProfileName::loud)
        return 3;

    return 0;
}

