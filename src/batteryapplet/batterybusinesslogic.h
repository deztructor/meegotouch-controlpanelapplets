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

public slots:
    void setPSMThresholdValue (int percentage);
    void setPSMValue (bool enabled);
    void setPSMAutoValue (bool toggle);
    void requestValues ();
    void remainingTimeValuesRequired ();

signals:
    void remainingTimeValuesChanged (QStringList values);
    // animation_rate is 0 when not charging...
    void batteryCharging (int animation_rate);
    void batteryBarValueReceived (int bar_value);
    void PSMValueReceived (bool enabled);

private slots:
    void batteryChargerEvent (Maemo::QmBattery::ChargerType type);
    void chargingStateChanged (Maemo::QmBattery::ChargingState state);
    void batteryStateChanged (Maemo::QmBattery::BatteryState batteryState);
    void batteryRemCapacityChanged (int percentage, int bars);
    void PSMStateChanged (Maemo::QmDeviceMode::PSMState state);

private:
    int batteryBarValue (int percentage);
    void recalculateChargingInfo ();
    bool realyCharging();

    bool              m_initialized;
    QmBattery        *m_battery;
    QmDeviceMode     *m_devicemode;
    int               m_ChargingRate;

#ifdef UNIT_TEST
    friend class Ut_BatteryBusinessLogic;
#endif
};

#endif
