/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include <QTimer>
#include <MGConfItem>
#include <QSettings>
#include <QVariant>
#include <QString>
#include <stdlib.h>

#undef DEBUG
#undef WARNING
#include "../debug.h"

static const QString GConfDir ("/system/osso/dsm/display");


using namespace MeeGo;

/*
 * gconftool --recursive-list /system/osso/dsm/display
 * gconftool --set /system/osso/dsm/display/use_low_power_mode --type boolean false
 *
 * gconftool --recursive-list /system/osso/dsm/locks
 * gconftool --set /system/osso/dsm/locks/tklock_double_tap_gesture --type integer 0
 */
static const QString LowPowerKey = GConfDir + "/use_low_power_mode";
static const QString DimTimeoutsKey (
        "/system/osso/dsm/display/possible_display_dim_timeouts");
static const QString DoubleTapKey (
        "/system/osso/dsm/locks/tklock_double_tap_gesture");

static const QString mcompositorConfName ("mcompositor");
static const QString closeFromTopKey ("swipe-action-down");
static const QString closeFromTopEnabled ("close");
static const QString closeFromTopDisabled ("away");

static int TIMEGAP = 5; // time gap between blanking and dimming

DisplayBusinessLogic::DisplayBusinessLogic (
        QObject* parent) :
    QObject (parent),
    m_Display (new MeeGo::QmDisplayState),
    m_compositorConf (0),
    m_psmValue (false)
{
    m_possibleDimValues = new MGConfItem (DimTimeoutsKey);
    m_lowPower = new MGConfItem (LowPowerKey);
    m_DoubleTap = new MGConfItem (DoubleTapKey);
    m_devicemode = new MeeGo::QmDeviceMode (this);

#ifndef MEEGO
    m_compositorConf = new QSettings (
        mcompositorConfName, mcompositorConfName);
#endif

    connect (m_lowPower, SIGNAL (valueChanged ()),
             SLOT (lpmValueChanged ()));
    connect (m_DoubleTap, SIGNAL (valueChanged ()),
             SLOT (doubleTapValueChanged ()));
    connect (m_devicemode,
             SIGNAL (devicePSMStateChanged (MeeGo::QmDeviceMode::PSMState)),
             SLOT (PSMStateChanged (MeeGo::QmDeviceMode::PSMState)));

    m_psmValue =
        (m_devicemode->getPSMState () == MeeGo::QmDeviceMode::PSMStateOn);
}

DisplayBusinessLogic::~DisplayBusinessLogic ()
{
    delete m_Display;
    m_Display = 0;

#ifndef MEEGO
    delete m_compositorConf;
    m_compositorConf = 0;
#endif

    delete m_possibleDimValues;
    m_possibleDimValues = 0;

    delete m_lowPower;
    m_lowPower = 0;

    delete m_DoubleTap;
    m_DoubleTap = 0;
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
    max = m_Display->getMaxDisplayBrightnessValue();

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
    return m_Display->getDisplayBrightnessValue();
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

    QList<int> values = screenLightsValues ();
    index = values.indexOf (m_Display->getDisplayDimTimeout ());

    if (index < 0) {
        index = 1;
        setScreenLightTimeouts (index);
    }

    return index;
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

    m_Display->setDisplayBrightnessValue (value + 1);
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
DisplayBusinessLogic::setLowPowerMode (
        bool enable)
{
    SYS_DEBUG ("enable = %s", SYS_BOOL (enable));
    m_lowPower->set (QVariant(enable));
}

void
DisplayBusinessLogic::setDoubleTapWakes (
        bool enable)
{
    SYS_DEBUG ("enable = %s", SYS_BOOL (enable));
    int ival = enable ? 1 : 0;

    m_DoubleTap->set (QVariant(ival));
}

bool
DisplayBusinessLogic::getLowPowerMode ()
{
    return m_lowPower->value(QVariant(false)).toBool();
}

bool
DisplayBusinessLogic::getDoubleTapWakes ()
{
    return m_DoubleTap->value().toInt() != 0;
}

void
DisplayBusinessLogic::lpmValueChanged ()
{
    bool value = m_lowPower->value().toBool();

    emit lowPowerModeChanged (value);
}

void
DisplayBusinessLogic::doubleTapValueChanged ()
{
    int value = m_DoubleTap->value().toInt();

    emit doubleTapModeChanged (value != 0);
}

/*!
 * This slot will be called when the device power save mode is changed. The
 * method will send the PSMValueReceived() signal.
 */
void
DisplayBusinessLogic::PSMStateChanged (
        MeeGo::QmDeviceMode::PSMState state)
{
    m_psmValue = (state == MeeGo::QmDeviceMode::PSMStateOn);

    SYS_DEBUG ("*** state = %d", (int)state);
    SYS_DEBUG ("Emitting PSMValueReceived (%s)", SYS_BOOL(enabled));
    emit PSMValueReceived (m_psmValue);
}

bool
DisplayBusinessLogic::PSMValue ()
{
    return m_psmValue;
}

void
DisplayBusinessLogic::setCloseFromTop (bool enable)
{
    SYS_DEBUG ("enable = %s", SYS_BOOL (enable));
#ifndef MEEGO
    QString val = enable ? closeFromTopEnabled : closeFromTopDisabled;

    bool oldVal = getCloseFromTopValue ();

    m_compositorConf->setValue (closeFromTopKey, val);
    m_compositorConf->sync ();

    // HACK: BEGIN
    if (enable != oldVal)
    {
        // Notify the mcompositor about the change
        SYS_DEBUG ("Notifing mcompositor...");
        system ("kill -SIGHUP $(pgrep mcompositor)");
    }
    // HACK: END
#endif
}

bool
DisplayBusinessLogic::getCloseFromTopValue ()
{
    bool retval = false;

    /*
     * TODO: FIXME:
     * Implement a tracker for this key to listen for
     * value-changes... (but most probably no-one will
     * modify this key...)
     */
#ifndef MEEGO
    QString setting = m_compositorConf->value (
        closeFromTopKey, QVariant (closeFromTopDisabled)).toString ();

    retval = (setting == closeFromTopEnabled);
#endif

    SYS_DEBUG ("returning %s", SYS_BOOL (retval));
    return retval;
}

