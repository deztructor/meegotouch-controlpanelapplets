/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "displaybusinesslogic.h"
#include <MGConfItem>

#undef DEBUG
#define WARNING
#include "../debug.h"

using namespace Maemo;

static int TIMEGAP = 5; // time gap between blanking and dimming

#define POSSIBLE_DIM_TIMEOUTS \
    "/system/osso/dsm/display/possible_display_dim_timeouts"

DisplayBusinessLogic::DisplayBusinessLogic (
        QObject* parent) :
    QObject (parent),
    m_Display (new QmDisplayState()),
    m_possibleDimValues (0)
{
    m_possibleDimValues = new MGConfItem (POSSIBLE_DIM_TIMEOUTS);
}

DisplayBusinessLogic::~DisplayBusinessLogic ()
{
    delete m_Display;
    m_Display = 0;
    delete m_possibleDimValues;
    m_possibleDimValues = 0;
}

#if 0
void
DisplayBusinessLogic::toggleDisplay (
        bool toggle)
{
    SYS_DEBUG ("*** toggle = %s", toggle ? "true" : "false");
    QmDisplayState::DisplayState state = 
        (toggle ? QmDisplayState::On : QmDisplayState::Off);

    m_Display->set (state);
}
#endif

/*!
 * Returns a list of brightness values the underlying hw system accepts.
 */
QList<int> 
DisplayBusinessLogic::brightnessValues ()
{
    QList<int> values;

    int max = m_Display->getMaxDisplayBrightnessValue();
    max = (max > 0 ? max : 5);

    // QmDisplayState::setDisplayBrightnessValue accepts values 
    // between 1 and getMaxDisplayBrightnessValue()
    for (int i = 1; i <= max; ++i) {
        SYS_DEBUG ("BrightnessValues: %d", i);
        values << i;
    }

    return values;
}

/*!
 * Returns the current brighness value for the screen. Ooops, this function
 * actually returns the index. FIXME: Should be checked and renamed.
 */
int
DisplayBusinessLogic::selectedBrightnessValue ()
{
    QList<int> values = brightnessValues();

    SYS_DEBUG ("*** getDisplayBrightnessValue() = %d", 
            m_Display->getDisplayBrightnessValue());
    int index = values.indexOf(m_Display->getDisplayBrightnessValue());
    if (index < 0) {
        index = values.size() / 2;
        setBrightnessValue (values.at(index));
    }

    SYS_DEBUG ("*** index                       = %d", index);
    return index;
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

int 
DisplayBusinessLogic::selectedScreenLightsValue ()
{
    QList<int> values = screenLightsValues ();
    int index = values.indexOf (m_Display->getDisplayDimTimeout ());

    if (index < 0) {
        index = 1;
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

/*!
 * \param value The slider value, that starts from 0, the qmsystem value starts
 *   from 1, so we add +1 to this parameter.
 */
void 
DisplayBusinessLogic::setBrightnessValue (
        int value)
{
    SYS_DEBUG ("*** value + 1 = %d", value + 1);
    m_Display->setDisplayBrightnessValue(value + 1);
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

