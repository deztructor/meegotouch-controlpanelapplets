/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "resetbrief.h"
#include <dcpwidgettypes.h>

#define DEBUG
#include "../debug.h"

ResetBrief::ResetBrief (
        ResetBusinessLogic *businessLogic) :
    m_ResetBusinessLogic (businessLogic)
{
}

int
ResetBrief::widgetTypeID() const
{
    return DcpWidgetType::Label;
}

QString 
ResetBrief::valueText() const
{
    return "";
}


