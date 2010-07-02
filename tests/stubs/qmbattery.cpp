/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "qmbattery.h"

#define DEBUG
#define WARNING
#include "../../src/debug.h"

using namespace Maemo;

QmBattery::QmBattery (
        QObject *parent) :
    QObject (parent),
    m_ChargerType (None),
    m_BatteryState (StateOK),
    m_ChargingState (StateNotCharging),
    m_Percentage (50)
{
    SYS_DEBUG ("Creating %p", this);
}

QmBattery::~QmBattery ()
{
    SYS_DEBUG ("Destroying %p", this);
}

        
int
QmBattery::getRemainingCapacityPct() const
{
    SYS_DEBUG ("Returning %d%%", m_Percentage);
    return m_Percentage;
}

QmBattery::ChargingState
QmBattery::getChargingState() const
{
    SYS_DEBUG ("Returning %s", SYS_STR(ChargingStateName(m_ChargingState)));
    return m_ChargingState;
}

int
QmBattery::getRemainingIdleTime(
        RemainingTimeMode mode) const
{
    Q_UNUSED (mode);
    return 231;
}

QmBattery::BatteryState
QmBattery::getBatteryState() const
{
    SYS_DEBUG ("Returning %s", SYS_STR(BatteryStateName(m_BatteryState)));
    return m_BatteryState;
}

int
QmBattery::getRemainingTalkTime (
        RemainingTimeMode mode) const
{
    Q_UNUSED (mode);
    return 1200;
}


QmBattery::ChargerType 
QmBattery::getChargerType() const
{
    SYS_DEBUG ("returning %s", SYS_STR(ChargerName(m_ChargerType)));
    return m_ChargerType;
}

int
QmBattery::getMaxBars() const
{
    return 9;
}

/******************************************************************************
 * Low level functions to help the testing. These are not in the original!
 */
void
QmBattery::connectCharger (
        ChargerType chargerType)
{
    SYS_DEBUG ("Changing charger %s -> %s", 
            SYS_STR(ChargerName(m_ChargerType)),
            SYS_STR(ChargerName(chargerType)));

    if (chargerType == m_ChargerType)
        return;

    m_ChargerType = chargerType;

    SYS_DEBUG ("Emitting chargerEvent(%s)", SYS_STR(ChargerName(chargerType)));
    emit chargerEvent (chargerType);
}

void 
QmBattery::modifyBatteryState (
        BatteryState BatteryState, 
        int          percentage)
{
    SYS_DEBUG ("Chaging batteryState %s -> %s",
            SYS_STR(BatteryStateName(m_BatteryState)),
            SYS_STR(BatteryStateName(BatteryState)));
    SYS_DEBUG ("Changing percentage  %d%% -> %d%%", m_Percentage, percentage);

    if (percentage != m_Percentage) {
        m_Percentage = percentage;
        SYS_DEBUG ("Emitting batteryRemainingCapacityChanged (%d, %d)",
                percentage, percentage / 10);
        emit batteryRemainingCapacityChanged (percentage, percentage / 10);
    }

    if (BatteryState != m_BatteryState) {
        m_BatteryState = BatteryState;
        SYS_DEBUG ("Emitting batteryStateChanged (%s)",
                SYS_STR(BatteryStateName(BatteryState)));
        emit batteryStateChanged (BatteryState);
    }
}

void
QmBattery::setChargingState (
        ChargingState    newState)
{
    SYS_DEBUG ("Chaging batteryState %s -> %s",
            SYS_STR(ChargingStateName(m_ChargingState)),
            SYS_STR(ChargingStateName(newState)));

    if (newState == m_ChargingState)
        return;
    m_ChargingState = newState;

    SYS_DEBUG ("Emitting chargingStateChanged (%s)", 
            SYS_STR(ChargingStateName(newState)));
    emit chargingStateChanged (newState);
}

QString
QmBattery::ChargerName (
        ChargerType chargerType) const
{
    switch (chargerType) {
        case Unknown:
            return "QmBattery::Unknown";
        case None:
            return "QmBattery::None";
        case Wall:
            return "QmBattery::Wall";
        case USB_500mA:
            return "QmBattery::USB_500mA";
        case USB_100mA:
            return "QmBattery::USB_100mA";
    }

    return "NOSUCHACHARGER";
}

QString
QmBattery::BatteryStateName (
        BatteryState  batteryState) const
{
    switch (batteryState) {
        case StateEmpty:
            return "QmBattery::StateEmpty";
        case StateLow:
            return "QmBattery::StateLow";
        case StateOK:
            return "QmBattery::StateOK";
        case StateFull:
            return "QmBattery::StateFull";
        case StateError:
            return "QmBattery::StateError";
    }

    return "NOSUCHBATTERYSTATE";
}

QString
QmBattery::ChargingStateName (
        ChargingState state) const
{
    switch (state) {
        case StateNotCharging:
            return "QmBattery::StateNotCharging";
        case StateCharging:
            return "QmBattery::StateCharging";
        case StateChargingFailed:
            return "QmBattery::StateChargingFailed";
    }

    return "NOSUUCHCHARGINGSTATE";
}
