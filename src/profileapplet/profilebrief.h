/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef PROFILEBRIEF_H
#define PROFILEBRIEF_H

#include <QPointer>
#include "dcpbrief.h"

class ProfileDataInterface;

class ProfileBrief: public DcpBrief
{
    Q_OBJECT

public:
    ProfileBrief (ProfileDataInterface *profileDataInterfac);
    virtual QString valueText() const;

public slots:
    void currentProfileChanged (int id);

private:
    QPointer<ProfileDataInterface> m_Api;
};

#endif 
