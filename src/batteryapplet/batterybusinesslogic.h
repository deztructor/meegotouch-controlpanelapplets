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
#ifndef BATTERYBUSINESSLOGIC_H
#define BATTERYBUSINESSLOGIC_H

#include <QObject>
#include <QStringList>

#include <qmdevicemode.h>
#include <qmbattery.h>

using namespace Maemo;

class BatteryBusinessLogic : public QObject
{
    Q_OBJECT

public:
    BatteryBusinessLogic (QObject *parent = 0);
    ~BatteryBusinessLogic ();
    
    QStringList PSMThresholdValues ();
    int PSMThresholdValue ();
    bool PSMAutoValue ();
    bool isCharging();

public slots:
    void setPSMThresholdValue (int percentage);
    void setPSMValue (bool enabled);
    void setPSMAutoValue (bool toggle);
    void requestValues ();
//    void remainingTimeValuesRequired ();
    void remainingCapacityRequired ();

signals:
    void remainingTimeValuesChanged (QStringList values);
    void remainingBatteryCapacityChanged (int value);
    // animation_rate is 0 when not charging...
    void batteryCharging (int animation_rate);
    void batteryBarValueReceived (int bar_value);
    void PSMValueReceived (bool enabled);
    void batteryFull ();

private slots:
    void batteryChargerEvent (Maemo::QmBattery::ChargerType type);
    void chargingStateChanged (Maemo::QmBattery::ChargingState state);
    void batteryStateChanged (Maemo::QmBattery::BatteryState batteryState);
    void batteryRemCapacityChanged (int percentage, int bars);
    void PSMStateChanged (Maemo::QmDeviceMode::PSMState state);

private:
    int batteryBarValue (int percentage);
    void recalculateChargingInfo ();

    bool              m_initialized;
    QmBattery        *m_battery;
    QmDeviceMode     *m_devicemode;

    // If the charger is connected, the battery is not full and the charging is
    // not failed this integer shows the speed of the charging animation.
    int               m_ChargingRate;

    // If the charger is connected. Might be not really charging if the battery
    // is full.
    bool              m_Charging;
    
#ifdef UNIT_TEST
    friend class Ut_BatteryBusinessLogic;
#endif
};

#endif
