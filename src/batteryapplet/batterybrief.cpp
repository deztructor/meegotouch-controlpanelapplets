/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#include "batterybrief.h"
#include <dcpwidgettypes.h>

#define DEBUG
#include "../debug.h"

int
BatteryBrief::widgetTypeID() const
{
    return DcpWidgetType::Label;
}

