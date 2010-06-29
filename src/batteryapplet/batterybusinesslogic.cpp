/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary (-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of battery-applet.
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
#include "batterybusinesslogic.h"
#include <MGConfItem>
#include <QVariant>
#include <QString>

#undef DEBUG
#define WARNING
#include "../debug.h"

static const char *psm_auto_key =
    "/system/osso/dsm/energymanagement/enable_power_saving";
static const char *psm_values_key =
    "/system/osso/dsm/energymanagement/possible_psm_thresholds";

static const int animation_rate_charging_usb  = 500;
static const int animation_rate_charging_wall = 250;


BatteryBusinessLogic::BatteryBusinessLogic (QObject *parent)
    : QObject (parent),
    m_initialized (false)
{
    m_battery = new Maemo::QmBattery (this);
    m_devicemode = new Maemo::QmDeviceMode (this);
}

BatteryBusinessLogic::~BatteryBusinessLogic ()
{

}

/*
 * This function does the initialization and signal
 * connection to QmBattery and QmDeviceMode, and emits
 * all the signals with the current values...
 */
void
BatteryBusinessLogic::requestValues ()
{
    if (m_initialized)
        return;

    m_initialized = true;

    connect (m_battery,
             SIGNAL (chargerEvent (Maemo::QmBattery::ChargerType)),
             this,
             SLOT (batteryChargerEvent (Maemo::QmBattery::ChargerType)));
    connect (m_battery,
             SIGNAL (chargingStateChanged  (Maemo::QmBattery::ChargingState)),
             this,
             SLOT (chargingStateChanged (Maemo::QmBattery::ChargingState)));
    // This will emit the batteryCharging signal,
    // and the remainingTimeValuesChanged signal
    chargingStateChanged (m_battery->getChargingState ());

    connect (m_battery,
             SIGNAL (batteryRemainingCapacityChanged (int, int)),
             this,
             SLOT (batteryRemCapacityChanged (int, int)));

    // batteryBarValueReceived also emitted by chargingStateChanged ^^^

    connect (m_devicemode,
             SIGNAL (devicePSMStateChanged (Maemo::QmDeviceMode::PSMState)),
             this,
             SLOT (PSMStateChanged (Maemo::QmDeviceMode::PSMState)));

    emit PSMValueReceived (m_devicemode->getPSMState () ==
                           Maemo::QmDeviceMode::PSMStateOn);
}

void
BatteryBusinessLogic::setPSMThresholdValue (int percentage)
{
    bool ret;
    ret = m_devicemode->setPSMBatteryMode (percentage);

    if (! ret)
        SYS_WARNING (" failed to set (precentage = %d)", percentage);
}

int
BatteryBusinessLogic::PSMThresholdValue ()
{
    return m_devicemode->getPSMBatteryMode ();
}

QStringList
BatteryBusinessLogic::PSMThresholdValues ()
{
    MGConfItem possible_values (psm_values_key);

    return possible_values.value ().toStringList ();
}

void
BatteryBusinessLogic::setPSMValue (bool enabled)
{
    bool ret;
    ret = m_devicemode->setPSMState (
        enabled ?
        Maemo::QmDeviceMode::PSMStateOn :
        Maemo::QmDeviceMode::PSMStateOff);

    if (ret)
    {
        // Change succeed, we don't need to wait for QmSystem
        // reply, we can emit the PSMValueChanged asap to
        // update the UI.
        emit PSMValueReceived (enabled);
    }
}

void
BatteryBusinessLogic::setPSMAutoValue (bool toggle)
{
    MGConfItem PSMAutoKey (psm_auto_key);

    PSMAutoKey.set (toggle);
}

bool
BatteryBusinessLogic::PSMAutoValue ()
{
    MGConfItem PSMAutoKey (psm_auto_key);

    return PSMAutoKey.value ().toBool ();
}

void
BatteryBusinessLogic::remainingTimeValuesRequired ()
{
    QStringList values;

    if (m_battery->getChargingState () == Maemo::QmBattery::StateCharging)
    {
        //% "Charging"
        values << qtTrId ("qtn_ener_charging") 
               << qtTrId ("qtn_ener_charging");
    }
    else
    {
        Maemo::QmDeviceMode::PSMState state = m_devicemode->getPSMState ();
        Maemo::QmBattery::RemainingTimeMode mode;
        switch (state)
        {
            case Maemo::QmDeviceMode::PSMStateOn:
                mode = Maemo::QmBattery::PowersaveMode;
                break;

            case Maemo::QmDeviceMode::PSMStateOff:
            default:
                mode = Maemo::QmBattery::NormalMode;
                break;
        }
        values << QString ("%1").arg (m_battery->getRemainingTalkTime (mode) / 60)
           << QString ("%1").arg (m_battery->getRemainingIdleTime (mode) / 60);
    }

    emit remainingTimeValuesChanged (values);
}

void
BatteryBusinessLogic::batteryChargerEvent (Maemo::QmBattery::ChargerType type)
{
    Q_UNUSED (type);

    chargingStateChanged (m_battery->getChargingState ());
}

void
BatteryBusinessLogic::chargingStateChanged (Maemo::QmBattery::ChargingState state)
{
    int rate = 0;

    /*
     * No animation needed when battery is full
     */
    if (m_battery->getBatteryState () != Maemo::QmBattery::StateFull)
    {
        switch (state)
        {
            case Maemo::QmBattery::StateCharging:
                rate = animation_rate_charging_usb;

                if (m_battery->getChargerType () == Maemo::QmBattery::Wall)
                    rate = animation_rate_charging_wall;

                break;

            case Maemo::QmBattery::StateNotCharging:
            case Maemo::QmBattery::StateChargingFailed:
            default:
                break;
        }
    }

    emit batteryCharging (rate);
    emit batteryBarValueReceived (batteryBarValue (-1));
    remainingTimeValuesRequired ();
}

void
BatteryBusinessLogic::PSMStateChanged (Maemo::QmDeviceMode::PSMState state)
{
    bool enabled = false;

    if (state == Maemo::QmDeviceMode::PSMStateOn)
        enabled = true;

    emit PSMValueReceived (enabled);
}

void
BatteryBusinessLogic::batteryRemCapacityChanged (int percentage, int bars)
{
    Q_UNUSED (bars);
    SYS_DEBUG ("percentage = %d, bars = %d [max = %d]",
               percentage, bars, m_battery->getMaxBars ());
    // XXX: FIXME: maybe we can drop batteryBarValue and use 'bars' parameter..

    emit batteryBarValueReceived (batteryBarValue (percentage));

    remainingTimeValuesRequired ();
}

/*!
 * \param percentage The energy level percentage or -1 to ask the backend.
 *
 * Returns the bar value (the value that used as an icon index representing for
 * the battery energy level) for the given percentage level. If the argument is
 * -1 this method will ask the QmBattery for the current energy level.
 *
 */
int
BatteryBusinessLogic::batteryBarValue (int percentage)
{
    int index;

    if (percentage == -1)
    {
        percentage = m_battery->getRemainingCapacityPct ();
    }

    // in case of overflow (eg. in sbox) when we get value that greater than 100
    // percent we use 10 percent intentionaly
    if (percentage < 0)
        percentage = 0;
    else if (percentage > 100)
        percentage = 10;

    if (percentage >= 84)
        index = 9;
    else if (percentage < 84 && percentage >= 73)
        index = 8;
    else if (percentage < 73 && percentage >= 62)
        index = 7;
    else if (percentage < 62 && percentage >= 51)
        index = 6;
    else if (percentage < 51 && percentage >= 39)
        index = 5;
    else if (percentage < 39 && percentage >= 28)
        index = 4;
    else if (percentage < 28 && percentage >= 17)
        index = 3;
    else if (percentage < 17 && percentage >= 5)
        index = 2;
    else if (percentage < 5 && percentage > 1)
        index = 1;
    else // if (percentage == 0)
        index = 0;

    return index;
}

