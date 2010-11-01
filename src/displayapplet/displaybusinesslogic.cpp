/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of meegotouch-controlpanelapplets.
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

#include "displaybusinesslogic.h"
#include <MGConfItem>

#undef DEBUG
#define WARNING
#include "../debug.h"

#ifdef HAVE_QMSYSTEM
using namespace MeeGo;
#else
static const QString GConfDir ("/system/osso/dsm/display");
static const QString MaxBrightnessKey = 
    GConfDir + "/max_display_brightness_levels";
static const QString CurrentBrightnessKey = 
    GConfDir + "/display_brightness";
#endif

static int TIMEGAP = 5; // time gap between blanking and dimming

#define POSSIBLE_DIM_TIMEOUTS \
    "/system/osso/dsm/display/possible_display_dim_timeouts"

#ifdef HAVE_QMSYSTEM
DisplayBusinessLogic::DisplayBusinessLogic (
        QObject* parent) :
    QObject (parent),
    m_Display (new QmDisplayState())
{
    m_possibleDimValues = new MGConfItem (POSSIBLE_DIM_TIMEOUTS);
}
#else
DisplayBusinessLogic::DisplayBusinessLogic (
        QObject* parent) :
    QObject (parent)
{
    m_MaxDisplayBrightness = new MGConfItem (MaxBrightnessKey);
    m_CurrentBrightness = new MGConfItem (CurrentBrightnessKey);
    m_possibleDimValues = new MGConfItem (POSSIBLE_DIM_TIMEOUTS);
}
#endif

DisplayBusinessLogic::~DisplayBusinessLogic ()
{
    #ifdef HAVE_QMSYSTEM
    delete m_Display;
    m_Display = 0;
    #else
    delete m_MaxDisplayBrightness;
    m_MaxDisplayBrightness = 0;

    delete m_CurrentBrightness;
    m_CurrentBrightness = 0;
    #endif

    delete m_possibleDimValues;
    m_possibleDimValues = 0;
}

/*!
 * Returns a list of brightness values the underlying hw system accepts.
 */
QList<int> 
DisplayBusinessLogic::brightnessValues ()
{
    QList<int> values;
    int        max;

    /*
     * If the QmSystem is available we use it to get the maximum brightness
     * value, if not we read this value from the GConf database.
     */
    #ifdef HAVE_QMSYSTEM
    max = m_Display->getMaxDisplayBrightnessValue();
    #else
    max = m_MaxDisplayBrightness->value().toInt();
    #endif
    
    // Well, this must be some kind of last minute check...
    max = (max > 0 ? max : 5);

    /*
     * What we use here is a list of accepted values from 1 to the maximum
     * value.
     */
    for (int i = 1; i <= max; ++i) {
        SYS_DEBUG ("*** BrightnessValues[%d] = %d", i - 1, i);
        values << i;
    }

    return values;
}

/*!
 * Returns the current brighness value for the screen. Ooops, this function
 * actually returns the index. FIXME: Should be checked and renamed.
 */
int
DisplayBusinessLogic::selectedBrightnessValueIndex ()
{
    QList<int> values = brightnessValues();
    int        index;

    index = values.indexOf (selectedBrightnessValue());
    if (index < 0) {
        SYS_WARNING ("Value not in available values list.");
        index = values.size() / 2;
        setBrightnessValue (values.at(index));
    }

    SYS_DEBUG ("*** index                       = %d", index);
    return index;
}

int
DisplayBusinessLogic::selectedBrightnessValue ()
{
    /*
     * When we have the QmSystem we use that, otherwise we read the data from
     * the GConf database.
     */
    #ifdef HAVE_QMSYSTEM
    return m_Display->getDisplayBrightnessValue();
    #else
    return m_CurrentBrightness->value().toInt();
    #endif
}

/*!
 * Returns a list that contains the available touch screen backlight time-out 
 * values. All the elements are measuring timeout values in seconds.
 */
QList<int>
DisplayBusinessLogic::screenLightsValues ()
{
    QList<int> values;

    QList<QVariant> gconf_values =
        m_possibleDimValues->value().toList ();

    if (gconf_values.isEmpty ())
    {
        values << 15 << 30 << 60 << 120 << 180;
        SYS_WARNING ("GConf doesn't have possible dim "
                     "values, using the default list");
    }
    else
    {
        SYS_DEBUG ("Possible dim values from GConf:");
        foreach (QVariant i, gconf_values)
        {
            values << i.toInt ();
            SYS_DEBUG ("%d", i.toInt ());
        }
    }

    return values;
}

/*!
 * FIXME: The name of the method should be modified: this method actually
 * returns the index of the screen dim timeout value.
 */
int 
DisplayBusinessLogic::selectedScreenLightsValue ()
{
    int index;

    #if HAVE_QMSYSTEM
    QList<int> values = screenLightsValues ();
    index = values.indexOf (m_Display->getDisplayDimTimeout ());

    if (index < 0) {
        index = 1;
        setScreenLightTimeouts (index);
    }
    #else
    /*
     * FIXME: To add the code that returns the selected screen lights value when
     * QmSystem is not available.
     */
    SYS_WARNING ("Not implemented!");
    index = 0;
    #endif
    return index;
}

bool 
DisplayBusinessLogic::blankInhibitValue ()
{
    /*
     * Actually the UI string is a negative.
     */
    #if HAVE_QMSYSTEM
    return !m_Display->getBlankingWhenCharging();
    #else
    /*
     * FIXME: To add the code that gets the value when QmSystem is not
     * available.
     */
    SYS_WARNING ("Not implemented!");
    return false;
    #endif
}

/*!
 * \param value The slider value, that starts from 0, the qmsystem value starts
 *   from 1, so we add +1 to this parameter.
 */
void 
DisplayBusinessLogic::setBrightnessValue (
        int value)
{
    SYS_DEBUG ("*** value + 1 = %d", value + 1);
    #if HAVE_QMSYSTEM
    m_Display->setDisplayBrightnessValue (value + 1);
    #else
    /*
     * FIXME: To add the code that is used when QmSystem is not available.
     */
    SYS_WARNING ("Not implemented!");
    #endif
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
    #ifdef HAVE_QMSYSTEM
    m_Display->setDisplayDimTimeout (seconds);
    /*
     * Originally it was seconds + TIMEGAP here, but as I see the blank timeout
     * is relative to the dim timeout value.
     */
    m_Display->setDisplayBlankTimeout (TIMEGAP);
    #else
    /*
     * FIXME: To add the code that is used when there is no QmSystem.
     */
    SYS_WARNING ("Not implemented!");
    #endif
}

/*!
 * \param value If true the screen lights will remain on while the charger is
 *   connected.
 */
void 
DisplayBusinessLogic::setBlankInhibitValue (
        bool value)
{
    /*
     * Actually the UI string is a negative 'keep the lights on'.
     */
    value = !value;
    SYS_DEBUG ("*** blanking when charging = %s", value ? "true" : "false");
    #ifdef HAVE_QMSYSTEM
    m_Display->setBlankingWhenCharging(value);
    #else
    /*
     * FIXME: To add the code that is used when there is no QmSystem.
     */
    SYS_WARNING ("Not implemented!");
    #endif
}

