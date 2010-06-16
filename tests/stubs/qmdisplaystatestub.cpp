/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of systemui.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/
#include "qmdisplaystatestub.h"

#define DEBUG
#include "../../src/debug.h"

using namespace Maemo;

QmDisplayState::QmDisplayState ()
{
    SYS_DEBUG ("Constructor: %p", this);
    m_State = On;
    m_DisplayBrightnessValue = 1;
    m_DisplayDimTimeout = 1;
    m_DisplayBlankTimeout = 1;
    m_BlankingWhenCharging = true;
}

QmDisplayState::~QmDisplayState ()
{
    SYS_DEBUG ("Destructor : %p", this);
}

bool
QmDisplayState::set(
        DisplayState state)
{
    m_State = state;
    return true;
}

int
QmDisplayState::getMaxDisplayBrightnessValue ()
{
    return 10;
}

int 
QmDisplayState::getDisplayBrightnessValue()
{
    SYS_DEBUG ("returning %d", m_DisplayBrightnessValue);
    return m_DisplayBrightnessValue;
}

int 
QmDisplayState::getDisplayDimTimeout()
{
    return m_DisplayDimTimeout;
}

bool
QmDisplayState::getBlankingWhenCharging()
{
    return m_BlankingWhenCharging;
}

void
QmDisplayState::setDisplayBrightnessValue(
        int brightness)
{
    m_DisplayBrightnessValue = brightness;
}

void
QmDisplayState::setDisplayDimTimeout(
        int timeout)
{
    m_DisplayDimTimeout = timeout;
}

void
QmDisplayState::setDisplayBlankTimeout(
        int timeout)
{
    m_DisplayBlankTimeout = timeout;
}

void
QmDisplayState::setBlankingWhenCharging(
        bool blanking)
{
    m_BlankingWhenCharging = blanking;
}

