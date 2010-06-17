/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#include "batterybrief.h"
#include <dcpwidgettypes.h>

#define DEBUG
#include "../debug.h"


/*
 * We don't want to anything at the second line unless we can show something
 * meaningful. Actually the UI spec does not specify what to show...
 */
#if 0
QString BatteryBrief::valueText() const
{
    return "Implemented";
}
#endif

int
BatteryBrief::widgetTypeID() const
{
    return DcpWidgetType::Label;
}

