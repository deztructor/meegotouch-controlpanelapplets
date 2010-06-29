/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of systemui-applets.
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
#include <MGConfItem>
#include "ut_batterybusinesslogic.h"

#define DEBUG
#include "../debug.h"

/*
 * Include the stubs here:
 */
#include "qmbattery_stub.h"
#include "qmdevicemode_stub.h"

QVariant GconfItemSet;

void MGConfItem::set (const QVariant &val)
{
    GconfItemSet = val;
}

enum _testCase
{
    CASE_BOOL,
    CASE_STRINGLIST
} testCase;

QVariant MGConfItem::value () const
{
    switch (testCase)
    {
        case CASE_BOOL:
            return QVariant (true);
            break;
        case CASE_STRINGLIST:
        {
            QStringList list;
            list << "10" << "20" << "30";
            return QVariant (list);
            break;
        }
    }

    return QVariant ();
}

/*
 * These are run before and after every test case
 */
void Ut_BatteryBusinessLogic::init ()
{
    /* Reset and set default return values to QmDeviceMode stub */
    gQmDeviceModeStub->stubReset ();
    gQmDeviceModeStub->stubSetReturnValue<int> ("getPSMBatteryMode", 10);
    gQmDeviceModeStub->stubSetReturnValue<Maemo::QmDeviceMode::PSMState> (
        "getPSMState", Maemo::QmDeviceMode::PSMStateOff);
    /* Reset and set default return values to QmBattery stub */
    gQmBatteryStub->stubReset ();
    gQmBatteryStub->stubSetReturnValue<Maemo::QmBattery::ChargerType> (
        "getChargerType", Maemo::QmBattery::None);
    gQmBatteryStub->stubSetReturnValue<Maemo::QmBattery::ChargingState> (
        "getChargingState", Maemo::QmBattery::StateNotCharging);
    gQmBatteryStub->stubSetReturnValue<Maemo::QmBattery::BatteryState> (
        "getBatteryState", Maemo::QmBattery::StateOK);
    gQmBatteryStub->stubSetReturnValue<int> ("getRemainingTalkTime", 120);
    gQmBatteryStub->stubSetReturnValue<int> ("getRemainingIdleTime", 240);
    gQmBatteryStub->stubSetReturnValue<int> ("getRemainingCapacityPct", 44);
    gQmBatteryStub->stubSetReturnValue<int> ("getMaxBars", 10);

    m_logic = new BatteryBusinessLogic;
}

void Ut_BatteryBusinessLogic::cleanup ()
{
    delete m_logic;
}

/*
 * These are run once [start / finish]
 */
void Ut_BatteryBusinessLogic::initTestCase ()
{
}

void
Ut_BatteryBusinessLogic::cleanupTestCase ()
{
}

void
Ut_BatteryBusinessLogic::testRequestValues ()
{
    QSignalSpy sig_charging (m_logic, SIGNAL (batteryCharging (int)));
    QSignalSpy sig_timevals (m_logic, SIGNAL (remainingTimeValuesChanged (QStringList)));
    QSignalSpy sig_battbars (m_logic, SIGNAL (batteryBarValueReceived (int)));
    QSignalSpy sig_psmvalue (m_logic, SIGNAL (PSMValueReceived (bool)));

    QList<QVariant> arguments;

    m_logic->requestValues ();

    /*
     * Wait for a little... [signals needs some time...]
     */
    QTest::qWait (100);

    /* check for "batteryCharging (0)" */
    QVERIFY (sig_charging.count () > 0);
    arguments = sig_charging.takeFirst ();
    QCOMPARE (arguments.at (0).toInt (), 0);

    /* check for remainingTimeValuesChanged ("100", "200") */
    QVERIFY (sig_timevals.count () > 0);
    arguments = sig_timevals.takeFirst ();
    QStringList values = arguments.at (0).toStringList ();

    QCOMPARE (values.at (0), QString ("2")); /* 120 secs */
    QCOMPARE (values.at (1), QString ("4")); /* 240 secs */

    /* check for batteryBarValueReceived (X?) */
    QVERIFY (sig_battbars.count () > 0);
    arguments = sig_battbars.takeFirst ();
    QCOMPARE (arguments.at (0).toInt (),
              m_logic->batteryBarValue (44));

    QVERIFY (sig_psmvalue.count () > 0);
    arguments = sig_psmvalue.takeFirst ();
    QCOMPARE (arguments.at (0).toBool (), false);

    /* call requestValues () again... should not do anything... */
    sig_psmvalue.clear ();
    m_logic->requestValues ();

    QTest::qWait (10);

    QVERIFY (sig_psmvalue.count () == 0);
}

void
Ut_BatteryBusinessLogic::testPSMThresholdValue ()
{
    /* Test the getter function first */
    gQmDeviceModeStub->stubSetReturnValue<int> ("getPSMBatteryMode", 10);
    QCOMPARE (m_logic->PSMThresholdValue (), 10);

    /* Test the setter function */
    m_logic->setPSMThresholdValue (40);
    QCOMPARE (gQmDeviceModeStub->stubCallCount ("setPSMBatteryMode"), 1);
    QCOMPARE (gQmDeviceModeStub->stubLastCallTo (
                "setPSMBatteryMode").parameter<int> (0), 40);

    /* Test the GConf reader function */
    testCase = CASE_STRINGLIST;
    QStringList result = m_logic->PSMThresholdValues ();
    QCOMPARE (result.at (0), QString ("10"));
    QCOMPARE (result.at (1), QString ("20"));
    QCOMPARE (result.at (2), QString ("30"));
}

void
Ut_BatteryBusinessLogic::testPSMValue ()
{
    QSignalSpy sig_psmvalue (m_logic, SIGNAL (PSMValueReceived (bool)));

    m_logic->setPSMValue (true);
    QCOMPARE (gQmDeviceModeStub->stubCallCount ("setPSMState"), 1);
    QCOMPARE (gQmDeviceModeStub->stubLastParameters<Maemo::QmDeviceMode::PSMState> (0),
              Maemo::QmDeviceMode::PSMStateOn);
    /* tests whether PSMValueReceived signal emitted after a succesfully
     * mode change [stub always succeed]... */
    QTest::qWait (10);
    QCOMPARE (sig_psmvalue.count (), 1);
    QCOMPARE (sig_psmvalue.takeFirst ().at (0).toBool (), true);
    sig_psmvalue.clear ();

    m_logic->setPSMValue (false);
    QCOMPARE (gQmDeviceModeStub->stubCallCount ("setPSMState"), 2);
    QCOMPARE (gQmDeviceModeStub->stubLastParameters<Maemo::QmDeviceMode::PSMState> (0),
              Maemo::QmDeviceMode::PSMStateOff);
    /* tests whether PSMValueReceived signal emitted after a succesfully
     * mode change [stub always succeed]... */
    QTest::qWait (10);
    QCOMPARE (sig_psmvalue.count (), 1);
    QCOMPARE (sig_psmvalue.takeFirst ().at (0).toBool (), false);
    sig_psmvalue.clear ();

    /* test the slot of qmdevicemode signal... */
    m_logic->PSMStateChanged (Maemo::QmDeviceMode::PSMStateOn);
    QTest::qWait (10);
    QCOMPARE (sig_psmvalue.count (), 1);
    QCOMPARE (sig_psmvalue.takeFirst ().at (0).toBool (), true);
}

void
Ut_BatteryBusinessLogic::testPSMAutoValue ()
{
    m_logic->setPSMAutoValue (true);
    QCOMPARE (GconfItemSet, QVariant (true));

    m_logic->setPSMAutoValue (false);
    QCOMPARE (GconfItemSet, QVariant (false));

    testCase = CASE_BOOL;
    QCOMPARE (m_logic->PSMAutoValue (), true);
}

void
Ut_BatteryBusinessLogic::testRemainingTimeVals ()
{
    QSignalSpy sig_timevals (m_logic, SIGNAL (remainingTimeValuesChanged (QStringList)));

    /* Charging */
    gQmBatteryStub->stubSetReturnValue<Maemo::QmBattery::ChargingState> (
        "getChargingState", Maemo::QmBattery::StateCharging);
    m_logic->remainingTimeValuesRequired ();

    QCOMPARE (sig_timevals.count (), 1);
    QCOMPARE (sig_timevals.takeFirst (). at (0).toStringList (). at (0),
              qtTrId ("qtn_ener_charging"));

    sig_timevals.clear ();

    /* Not charging: power-save */
    gQmBatteryStub->stubSetReturnValue<Maemo::QmBattery::ChargingState> (
        "getChargingState", Maemo::QmBattery::StateNotCharging);
    gQmDeviceModeStub->stubSetReturnValue<Maemo::QmDeviceMode::PSMState> (
        "getPSMState", Maemo::QmDeviceMode::PSMStateOn);

    m_logic->remainingTimeValuesRequired ();

    QCOMPARE (sig_timevals.count (), 1);
    QCOMPARE (gQmBatteryStub->stubLastCall ().parameter
               <Maemo::QmBattery::RemainingTimeMode> (0),
              Maemo::QmBattery::PowersaveMode);

    sig_timevals.clear ();

    /* Not charging, and not power-save */
    gQmBatteryStub->stubSetReturnValue<Maemo::QmBattery::ChargingState> (
        "getChargingState", Maemo::QmBattery::StateNotCharging);
    gQmDeviceModeStub->stubSetReturnValue<Maemo::QmDeviceMode::PSMState> (
        "getPSMState", Maemo::QmDeviceMode::PSMStateOff);

    m_logic->remainingTimeValuesRequired ();

    QCOMPARE (sig_timevals.count (), 1);
    QCOMPARE (gQmBatteryStub->stubLastCall ().parameter
               <Maemo::QmBattery::RemainingTimeMode> (0),
              Maemo::QmBattery::NormalMode);
}

void
Ut_BatteryBusinessLogic::testChargingStateChanged ()
{
    QSignalSpy sig_charging (m_logic, SIGNAL (batteryCharging (int)));

    gQmBatteryStub->stubSetReturnValue<Maemo::QmBattery::ChargingState> (
        "getChargingState", Maemo::QmBattery::StateCharging);

    /* Charger type Wall */
    gQmBatteryStub->stubSetReturnValue<Maemo::QmBattery::ChargerType> (
        "getChargerType", Maemo::QmBattery::Wall);

    m_logic->batteryChargerEvent (Maemo::QmBattery::Wall);

    QTest::qWait (20);

    QVERIFY (sig_charging.count () > 0);
    QList<QVariant> arguments = sig_charging.takeFirst ();
    /* wall charger -> rate = 250 msec */
    QCOMPARE (arguments.at (0).toInt (), 250);

    sig_charging.clear ();

    /* Charget type USB */
    gQmBatteryStub->stubSetReturnValue<Maemo::QmBattery::ChargerType> (
        "getChargerType", Maemo::QmBattery::USB_500mA);

    m_logic->batteryChargerEvent (Maemo::QmBattery::USB_500mA);

    QTest::qWait (20);

    QVERIFY (sig_charging.count () > 0);
    arguments = sig_charging.takeFirst ();
    /* usb charger -> rate = 500 msec */
    QCOMPARE (arguments.at (0).toInt (), 500);

    sig_charging.clear ();

    /* Charging failed */
    gQmBatteryStub->stubSetReturnValue<Maemo::QmBattery::ChargingState> (
        "getChargingState", Maemo::QmBattery::StateChargingFailed);
    gQmBatteryStub->stubSetReturnValue<Maemo::QmBattery::ChargerType> (
        "getChargerType", Maemo::QmBattery::USB_500mA);

    m_logic->batteryChargerEvent (Maemo::QmBattery::USB_500mA);

    QTest::qWait (20);

    QVERIFY (sig_charging.count () > 0);
    arguments = sig_charging.takeFirst ();
    /* charging failed -> rate = 0 [no animation] */
    QCOMPARE (arguments.at (0).toInt (), 0);

}

void
Ut_BatteryBusinessLogic::testRemCapacityChange ()
{
    QSignalSpy sig_battbar (m_logic, SIGNAL (batteryBarValueReceived (int)));
    QSignalSpy sig_remtime (m_logic, SIGNAL (remainingTimeValuesChanged (QStringList)));

    m_logic->batteryRemCapacityChanged (0, 0);

    QTest::qWait (20);

    QCOMPARE (sig_remtime.count (), 1);
    QCOMPARE (sig_battbar.count (), 1);

    QCOMPARE (sig_battbar.takeFirst ().at (0).toInt (),
              m_logic->batteryBarValue (0));
}

void
Ut_BatteryBusinessLogic::testBatteryBarValues ()
{
    /* Test invalid values: */
    QVERIFY (m_logic->batteryBarValue (111) == 2);

    gQmBatteryStub->stubSetReturnValue<int> ("getRemainingCapacityPct", -1);
    QVERIFY (m_logic->batteryBarValue (-1) == 0);
    QVERIFY (gQmBatteryStub->stubLastCall ().name () == "getRemainingCapacityPct");

    /* Test percentage values in valid range
       >= 84 */
    QVERIFY (m_logic->batteryBarValue (99) == 9);
    /* 73 <= value < 84 */
    QVERIFY (m_logic->batteryBarValue (77) == 8);
    /* 62 <= value < 73 */
    QVERIFY (m_logic->batteryBarValue (66) == 7);
    /* 51 <= value < 62 */
    QVERIFY (m_logic->batteryBarValue (55) == 6);
    /* 39 <= value < 51 */
    QVERIFY (m_logic->batteryBarValue (44) == 5);
    /* 28 <= value < 39 */
    QVERIFY (m_logic->batteryBarValue (33) == 4);
    /* 17 <= value < 28 */
    QVERIFY (m_logic->batteryBarValue (22) == 3);
    /*  5 <= value < 17 */
    QVERIFY (m_logic->batteryBarValue (11) == 2);
    /*  0 <  value < 5 */
    QVERIFY (m_logic->batteryBarValue (3)  == 1);
    /* 0 */
    QVERIFY (m_logic->batteryBarValue (0)  == 0);
}

QTEST_MAIN (Ut_BatteryBusinessLogic);
