/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#ifndef BATTERYBRIEF_H
#define BATTERYBRIEF_H

#include <dcpbrief.h>

class BatteryBrief: public DcpBrief
{
    Q_OBJECT

public:
    virtual int widgetTypeID() const;
};

#endif
