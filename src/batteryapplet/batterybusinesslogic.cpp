/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
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

#define DEBUG
#define WARNING
#include "../debug.h"

static const char *psm_auto_key =
    "/system/osso/dsm/energymanagement/enable_power_saving";
static const char *psm_values_key =
    "/system/osso/dsm/energymanagement/possible_psm_thresholds";

static const int animation_rate_charging_usb  = 500;
static const int animation_rate_charging_wall = 250;


BatteryBusinessLogic::BatteryBusinessLogic (
        QObject *parent)
    : QObject (parent),
    m_initialized (false),
    m_ChargingRate (-1)
{
    m_battery    = new QmBattery (this);
    m_devicemode = new QmDeviceMode (this);
}

BatteryBusinessLogic::~BatteryBusinessLogic ()
{
}

/*
 * This function does the initialization and signal
 * connection to QmBattery and QmDeviceMode, and emits
 * all the signals with the current values...
 *
 * FIXME: This function should be called realize(9 or something...
 */
void
BatteryBusinessLogic::requestValues ()
{
    if (m_initialized)
        return;

    m_initialized = true;

    SYS_DEBUG ("Connecting to the signals of the QmBattery class");
    connect (
        m_battery, SIGNAL(chargerEvent(Maemo::QmBattery::ChargerType)),
        this, SLOT(batteryChargerEvent(Maemo::QmBattery::ChargerType)));
    connect (
        m_battery, SIGNAL(chargingStateChanged(Maemo::QmBattery::ChargingState)),
        this, SLOT(chargingStateChanged(Maemo::QmBattery::ChargingState)));
    connect (
        m_battery, SIGNAL(batteryStateChanged(Maemo::QmBattery::BatteryState)),
        this, SLOT (batteryStateChanged(Maemo::QmBattery::BatteryState)));

    // This will emit the batteryCharging signal,
    // and the remainingTimeValuesChanged signal
    chargingStateChanged (m_battery->getChargingState ());

    /*
     * We have two signals showing that the battery energy level has been
     * changed. We listen both of these signals.
     */
    SYS_DEBUG ("Connecting to the signals of the QmBattery class");
    connect (m_battery, SIGNAL (batteryRemainingCapacityChanged (int, int)),
            this, SLOT (batteryRemCapacityChanged (int, int)));


    // batteryBarValueReceived also emitted by chargingStateChanged ^^^
    // FIXME: Why?
    connect (m_devicemode,
             SIGNAL (devicePSMStateChanged (Maemo::QmDeviceMode::PSMState)),
             this, SLOT (PSMStateChanged (Maemo::QmDeviceMode::PSMState)));

    SYS_DEBUG ("Emitting PSMValueReceived(%s)",
            SYS_BOOL(m_devicemode->getPSMState () == QmDeviceMode::PSMStateOn));
    emit PSMValueReceived (m_devicemode->getPSMState () ==
                           QmDeviceMode::PSMStateOn);
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
BatteryBusinessLogic::setPSMValue (
        bool enabled)
{
    bool ret;
    ret = m_devicemode->setPSMState (
        enabled ?
        QmDeviceMode::PSMStateOn :
        QmDeviceMode::PSMStateOff);

    if (ret) {
        // Change succeed, we don't need to wait for QmSystem reply, we can emit
        // the PSMValueChanged asap to update the UI.
        SYS_DEBUG ("Emitting PSMValueReceived(%s)", SYS_BOOL(enabled));
        emit PSMValueReceived (enabled);
    }
}

/*!
 * Sets the PSMAutoValue, a boolean that sets if the device should go into power
 * save mode automatically. This function only changes a value in the GConf
 * database.
 */
void
BatteryBusinessLogic::setPSMAutoValue (
        bool toggle)
{
    MGConfItem PSMAutoKey (psm_auto_key);
    PSMAutoKey.set (toggle);
}

/*!
 * Reads the PSMAuto value, a boolean that sets if the device should go into
 * power save mode automatically. This method is only reads the GConf database.
 */
bool
BatteryBusinessLogic::PSMAutoValue ()
{
    MGConfItem PSMAutoKey (psm_auto_key);
    return PSMAutoKey.value ().toBool ();
}

/*!
 * 
 */
void
BatteryBusinessLogic::remainingTimeValuesRequired ()
{
    QmBattery::RemainingTimeMode mode;
    QStringList                  values;

    if (realyCharging()) {
        //% "Charging"
        values << qtTrId ("qtn_ener_charging") 
               << qtTrId ("qtn_ener_charging");
        goto finalize;
    }

    switch (m_devicemode->getPSMState ()) {
        case QmDeviceMode::PSMStateOn:
            mode = QmBattery::PowersaveMode;
            break;

        case QmDeviceMode::PSMStateOff:
        default:
             mode = QmBattery::NormalMode;
             break;
    }
    
    values << QString ("%1").arg (m_battery->getRemainingTalkTime (mode) / 60)
           << QString ("%1").arg (m_battery->getRemainingIdleTime (mode) / 60);

finalize:
    SYS_DEBUG ("Emitting remainingTimeValuesChanged()");
    emit remainingTimeValuesChanged (values);
}

void
BatteryBusinessLogic::batteryChargerEvent (
        Maemo::QmBattery::ChargerType type)
{
    Q_UNUSED (type);

    SYS_DEBUG ("");
    recalculateChargingInfo ();
    //chargingStateChanged (m_battery->getChargingState ());
}

void
BatteryBusinessLogic::chargingStateChanged (
        Maemo::QmBattery::ChargingState state)
{
    Q_UNUSED (state);
    SYS_DEBUG ("");
    recalculateChargingInfo ();
    #if 0
    int rate = 0;

    /*
     * No animation needed when battery is full
     */
    if (m_battery->getBatteryState () != QmBattery::StateFull) {
        switch (state) {
            case QmBattery::StateCharging:
                rate = animation_rate_charging_usb;

                if (m_battery->getChargerType () == QmBattery::Wall)
                    rate = animation_rate_charging_wall;

                break;

            case QmBattery::StateNotCharging:
            case QmBattery::StateChargingFailed:
            default:
                break;
        }
    }

    SYS_DEBUG ("Emitting batteryCharging(%d)", rate);
    emit batteryCharging (rate);

    SYS_DEBUG ("Emitting batteryBarValueReceived(%d)", batteryBarValue (-1));
    emit batteryBarValueReceived (batteryBarValue (-1));

    remainingTimeValuesRequired ();
    #endif
}

void 
BatteryBusinessLogic::batteryStateChanged (
        Maemo::QmBattery::BatteryState batteryState)
{
    Q_UNUSED (batteryState);
    recalculateChargingInfo ();
}

/*!
 * This slot will be called when the device power save mode is changed. The
 * method will send the PSMValueReceived() signal.
 */
void
BatteryBusinessLogic::PSMStateChanged (
        Maemo::QmDeviceMode::PSMState state)
{
    bool enabled = state == QmDeviceMode::PSMStateOn;
    
    SYS_DEBUG ("Emitting PSMValueReceived (%s)", SYS_BOOL(enabled));
    emit PSMValueReceived (enabled);
}

void
BatteryBusinessLogic::batteryRemCapacityChanged (
		int percentage, 
		int bars)
{
    Q_UNUSED (bars);
    SYS_DEBUG ("**** percentage = %d, bars = %d [max = %d]",
               percentage, bars, m_battery->getMaxBars ());

    // XXX: FIXME: maybe we can drop batteryBarValue and use 'bars' parameter..
    SYS_DEBUG ("Emitting batteryBarValueReceived(%d)",
            batteryBarValue (percentage));
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

/*!
 * This function is called whenever some change is detected around the battery
 * and its charger. This method will try to find out if the battery is actually
 * charging and if it is the method will calculate the charging animation speed.
 *
 * What we managed to find aboput the charging is:
 * 1) The battery is not charging (despite any other info returned) if the
 *    battery is full. See NB#172929 for further details.
 */
void
BatteryBusinessLogic::recalculateChargingInfo ()
{
    QmBattery::ChargingState chargingState;
    QmBattery::BatteryState  batteryState;
    QmBattery::ChargerType   chargerType;
    bool                     charging;
    int                      chargingRate;
   
    chargerType = m_battery->getChargerType ();
    chargingState = m_battery->getChargingState ();
    batteryState = m_battery->getBatteryState ();

    /*
     * Carefully calculating the charging rate, the animation rate of the
     * charging indicator.
     */
    charging = 
        batteryState != QmBattery::StateFull &&
        chargerType != QmBattery::None &&
        chargingState != QmBattery::StateNotCharging &&
        chargingState != QmBattery::StateChargingFailed;

    chargingRate = 0;
    if (charging) 
        chargingRate = chargerType == QmBattery::Wall ?
            animation_rate_charging_wall : animation_rate_charging_usb;

    SYS_DEBUG ("*** charging      = %s", SYS_BOOL(charging));
    SYS_DEBUG ("*** chargingRate  = %d", chargingRate);

    if (chargingRate == m_ChargingRate) 
        return;

    /*
     * If the charging rate has been changed we need to notify the ui with a
     * signal.
     */
    SYS_DEBUG ("*** chargingRate %d -> %d", m_ChargingRate, chargingRate);
    m_ChargingRate = chargingRate;       
    SYS_DEBUG ("Emitting batteryCharging(%d)", m_ChargingRate);
    emit batteryCharging (m_ChargingRate);

    /*
     * Then we need to notify everyone about the bar value. 
     * FIXME: Why exactly do we need that?
     */
    SYS_DEBUG ("Emitting batteryBarValueReceived(%d)", batteryBarValue (-1));
    emit batteryBarValueReceived (batteryBarValue (-1));

    /*
     * And the remaining time values has to be recalculated.
     */
    remainingTimeValuesRequired ();
}

/*!
 * Returns true if the businesslogic decided that the battery is actually
 * charging. Please check the documentation of recalculateChargingInfo() for
 * further information.
 */
bool
BatteryBusinessLogic::realyCharging ()
{
    return m_ChargingRate != 0;
}
