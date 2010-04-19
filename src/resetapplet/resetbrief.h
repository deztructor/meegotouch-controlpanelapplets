/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef RESETBRIEF_H
#define RESETBRIEF_H

#include <QPointer>
#include "dcpbrief.h"

#include "resetbusinesslogic.h"


class ResetBrief: public DcpBrief
{
    Q_OBJECT

public:
    ResetBrief (ResetBusinessLogic *businessLogic);

    virtual QString valueText() const;

private:
    QPointer<ResetBusinessLogic> m_ResetBusinessLogic;
};

#endif
