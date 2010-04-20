/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef ABOUTBRIEF_H
#define ABOUTBRIEF_H

#include <QPointer>
#include "dcpbrief.h"

#include "aboutbusinesslogic.h"


class AboutBrief: public DcpBrief
{
    Q_OBJECT

public:
    AboutBrief (AboutBusinessLogic *businessLogic);

    virtual QString valueText() const;

private:
    QPointer<AboutBusinessLogic> m_AboutBusinessLogic;
};

#endif
