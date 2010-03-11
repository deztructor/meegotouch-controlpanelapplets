/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "displaybusinesslogic.h"

#include <QDebug>

#define DEBUG
#include "../debug.h"

/*
    TODO:
    1) Make sure the screen lights values and default index are correct

*/

using namespace Maemo;

static int TIMEGAP = 5; // time gap between blanking and dimming

DisplayBusinessLogic::DisplayBusinessLogic (
        QObject* parent) :
    QObject (parent),
    m_Display (new QmDisplayState())
{
}

DisplayBusinessLogic::~DisplayBusinessLogic()
{
    delete m_Display;
    m_Display = 0;
}

void
DisplayBusinessLogic::toggleDisplay (
        bool toggle)
{
    SYS_DEBUG ("*** toggle = %s", toggle ? "true" : "false");
    QmDisplayState::DisplayState state = 
        (toggle ? QmDisplayState::On : QmDisplayState::Off);

    m_Display->set (state);
}

QList<int> 
DisplayBusinessLogic::brightnessValues ()
{
    QList<int> values;

    int max = m_Display->getMaxDisplayBrightnessValue();
    max = (max > 0 ? max : 5);

    // QmDisplayState::setDisplayBrightnessValue accepts values 
    // between 1 and getMaxDisplayBrightnessValue()
    for (int i = 1; i <= max; ++i)
        values << i;

    return values;
}

int
DisplayBusinessLogic::selectedBrightnessValue ()
{
    QList<int> values = brightnessValues();

    int index = values.indexOf(m_Display->getDisplayBrightnessValue());
    if (index < 0) {
        index = values.size() / 2;
        setBrightnessValue (values.at(index));
    }

    return index;
}

QList<int> DisplayBusinessLogic::screenLightsValues ()
{
    QList<int> values;
    values << 10 << 30 << 60 << 120 << 300;
    return values;
}

int 
DisplayBusinessLogic::selectedScreenLightsValue ()
{
    QList<int> values = screenLightsValues();
    int index = values.indexOf(m_Display->getDisplayDimTimeout());
    if (index < 0) {
        index = values.size() / 2;
        setScreenLightTimeouts (index);
    }

    return index;
}

bool 
DisplayBusinessLogic::blankInhibitValue ()
{
    /*
     * Actually the UI string is a negative.
     */
    return !m_Display->getBlankingWhenCharging();
}

void 
DisplayBusinessLogic::setBrightnessValue (
        int value)
{
    SYS_DEBUG ("*** value = %d", value);
    m_Display->setDisplayBrightnessValue(value);
}

/*!
 * This function will take an index of the 'screen lights off' list that stores
 * the screen dim delay values and sets the dim timeout and the blank timeout
 * accordingly.
 */
void 
DisplayBusinessLogic::setScreenLightTimeouts (
		int     index)
{
    /*
     * We got the index, not the value.
     */
    QList<int> values = screenLightsValues ();
    int seconds = values[index];
    
    SYS_DEBUG ("*** index   = %d", index);
    SYS_DEBUG ("*** seconds = %d", seconds);

    m_Display->setDisplayDimTimeout (seconds);
    /*
     * Originally it was seconds + TIMEGAP here, but as I see the blank timeout
     * is relative to the dim timeout value.
     */
    m_Display->setDisplayBlankTimeout (TIMEGAP);
}

void 
DisplayBusinessLogic::setBlankInhibitValue (
        bool value)
{
    /*
     * Actually the UI string is a negative 'keep the lights on'.
     */
    value = !value;
    SYS_DEBUG ("*** blanking when charging = %s", value ? "true" : "false");
    m_Display->setBlankingWhenCharging(value);
}

