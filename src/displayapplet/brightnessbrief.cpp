/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Karoliina T. Salminen <karoliina.t.salminen@nokia.com>
**
** This file is part of duicontrolpanel.
**
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include <DcpWidgetTypes>
#include "brightnessbrief.h"

#include "displaybusinesslogic.h"

BrightnessBrief::BrightnessBrief():
    m_logic (new DisplayBusinessLogic),
    m_brightness_vals (m_logic->brightnessValues ())
{
}

BrightnessBrief::~BrightnessBrief ()
{
    delete m_logic;
}

QVariant BrightnessBrief::value() const
{
    return m_logic->selectedBrightnessValueIndex ();
}

void BrightnessBrief::setValue(const QVariant& value)
{
    m_logic->setBrightnessValue (value.toInt());
}


int BrightnessBrief::minValue() const
{
    return 0;
}

int BrightnessBrief::maxValue() const
{
    return m_brightness_vals.size () - 1;
}

int BrightnessBrief::widgetTypeID() const
{
    return DcpWidgetType::Slider;
}

