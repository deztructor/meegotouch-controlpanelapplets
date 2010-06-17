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
    void batteryRemCapacityChanged (int percentage, int bars);
    void batteryChargerEvent (Maemo::QmBattery::ChargerType type);
    void chargingStateChanged (Maemo::QmBattery::ChargingState state);
    void PSMStateChanged (Maemo::QmDeviceMode::PSMState state);

private:
    int batteryBarValue (int percentage);

    bool                     m_initialized;
    Maemo::QmBattery        *m_battery;
    Maemo::QmDeviceMode     *m_devicemode;

#ifdef UNIT_TEST
    friend class Ut_BatteryBusinessLogic;
#endif
};

#endif
