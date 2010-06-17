/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef TESTBRIEF_H
#define TESTBRIEF_H

#include <QPointer>
#include "dcpbrief.h"

class TestBrief: public DcpBrief
{
    Q_OBJECT

public:
    TestBrief ();
    virtual QString valueText() const;
    virtual int widgetTypeID() const;
};

#endif
