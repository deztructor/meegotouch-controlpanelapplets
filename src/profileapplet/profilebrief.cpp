/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "profilebrief.h"
#include "dcpwidgettypes.h"
#include "profiledatainterface.h"

#undef DEBUG
#include "../debug.h"

ProfileBrief::ProfileBrief (
        ProfileDataInterface *profileDataInterface) :
    m_Api (profileDataInterface)
{
    connect (m_Api, SIGNAL(currentProfile(int)),
            this, SLOT(currentProfileChanged(int)));
}

void 
ProfileBrief::currentProfileChanged (
        int id)
{
    Q_UNUSED (id);

    SYS_DEBUG ("Emitting valuesChanged()");
    emit valuesChanged ();
}

QString 
ProfileBrief::valueText() const
{
    return m_Api->getCurrentProfileName();
}


