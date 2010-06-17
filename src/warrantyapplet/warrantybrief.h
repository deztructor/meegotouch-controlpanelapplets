/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef WARRANTYBRIEF_H
#define WARRANTYBRIEF_H

#include <QPointer>
#include "dcpbrief.h"

#include "warrantybusinesslogic.h"


class WarrantyBrief: public DcpBrief
{
    Q_OBJECT

public:
    WarrantyBrief (WarrantyBusinessLogic *businessLogic);

    virtual QString valueText() const;
    virtual int widgetTypeID() const;

private:
    QPointer<WarrantyBusinessLogic> m_WarrantyBusinessLogic;
};

#endif
