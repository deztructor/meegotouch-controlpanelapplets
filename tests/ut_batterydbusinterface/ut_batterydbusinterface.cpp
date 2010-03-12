/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#include "ut_batterydbusinterface.h"
#include "batterydbusinterface.h"
#include <DuiApplication>
#include <QString>
#include <QStringList>

#define DEBUG
#include "../../src/debug.h"

const int oneStepWaiting = 100;

/******************************************************************************
 * Ut_BatteryDbusInterfacePrivate implementation.
 */
Ut_BatteryDbusInterfacePrivate::Ut_BatteryDbusInterfacePrivate () :
    m_PSMValueArrived (false),
    m_PSMAutoValueArrived (false),
    m_ThresholdValueArrived (false),
    m_ThresholdValuesArrived (false),
    m_RemainingTimeValuesArrived (false)
{
}

void
Ut_BatteryDbusInterfacePrivate::PSMValueReceived (
        bool PSMValue)
{
    m_PSMValueArrived = true;
    m_PSMValue = PSMValue;
}

void
Ut_BatteryDbusInterfacePrivate::PSMAutoValueReceived (
        bool PSMAutoValue)
{
    m_PSMAutoValueArrived = true;
    m_PSMAutoValue = PSMAutoValue;
}

void
Ut_BatteryDbusInterfacePrivate::PSMThresholdValueReceived (
        QString ThresholdValue)
{
    m_ThresholdValueArrived = true;
    m_ThresholdValue = ThresholdValue;
}

void
Ut_BatteryDbusInterfacePrivate::PSMThresholdValuesReceived (
        QStringList ThresholdValues)
{
    m_ThresholdValuesArrived = true;
    m_ThresholdValues = ThresholdValues;
}

void
Ut_BatteryDbusInterfacePrivate::remainingTimeValuesReceived (
        QStringList RemainingTimeValues)
{
    m_RemainingTimeValuesArrived = true;
    m_RemainingTimeValues = RemainingTimeValues;
}

void
Ut_BatteryDbusInterfacePrivate::batteryBarValueReceived (
        int batteryBarValue)
{
    m_batteryBarValueReceived = true;
    m_batteryBarValue = batteryBarValue;
}

/******************************************************************************
 * Ut_BatteryDbusInterface implementation. 
 */
void 
Ut_BatteryDbusInterface::init()
{
}

void 
Ut_BatteryDbusInterface::cleanup()
{
}

DuiApplication *app;

void 
Ut_BatteryDbusInterface::initTestCase()
{

    int argc = 1;
    char* app_name = (char*) "./Ut_BatteryDbusInterface";
    app = new DuiApplication(argc, &app_name);

    m_priv = new Ut_BatteryDbusInterfacePrivate;
    m_BatteryDBusInterface = new BatteryDBusInterface;

    /*
     * Connecting to the signals of the battery dbus interface.
     */
    connect (m_BatteryDBusInterface, SIGNAL (PSMValueReceived(bool)),
        m_priv, SLOT(PSMValueReceived(bool)));
    
    connect (m_BatteryDBusInterface, SIGNAL (PSMAutoValueReceived(bool)),
        m_priv, SLOT(PSMAutoValueReceived(bool)));

    connect (m_BatteryDBusInterface, SIGNAL (PSMThresholdValueReceived(QString)),
        m_priv, SLOT(PSMThresholdValueReceived (QString)));

    connect (m_BatteryDBusInterface, SIGNAL (PSMThresholdValuesReceived(QStringList)),
        m_priv, SLOT(PSMThresholdValuesReceived (QStringList)));

    connect (m_BatteryDBusInterface, SIGNAL (remainingTimeValuesReceived(QStringList)),
        m_priv, SLOT(remainingTimeValuesReceived (QStringList)));

    connect (m_BatteryDBusInterface, SIGNAL (batteryBarValueReceived (int)),
        m_priv, SLOT(batteryBarValueReceived (int)));

    m_BatteryDBusInterface->PSMValueRequired();
    m_BatteryDBusInterface->PSMAutoValueRequired();
    m_BatteryDBusInterface->PSMThresholdValuesRequired();
    m_BatteryDBusInterface->PSMThresholdValueRequired();
    m_BatteryDBusInterface->remainingTimeValuesRequired();
    m_BatteryDBusInterface->batteryBarValueRequired();
}

void 
Ut_BatteryDbusInterface::cleanupTestCase()
{
    delete m_priv;
    m_priv = 0;

    delete m_BatteryDBusInterface;
    m_BatteryDBusInterface = 0;

    delete app;
}

void 
Ut_BatteryDbusInterface::testSysuidRunning ()
{
    qDebug() << "--- What about the sysuid? Is it running? ------";
    system ("ps axu | grep sysuid");
    qDebug() << "------------------------------------------------";
    system ("pidof sysuid");
    qDebug() << "------------------------------------------------";
}


void 
Ut_BatteryDbusInterface::testGetPSMValue ()
{
    bool success;

    success = waitforit ("PSMValueArrived", &m_priv->m_PSMValueArrived);
    QVERIFY (success);
    
    qDebug() << "*** PSMValue    = " << 
            (m_priv->m_PSMValue ? "true" : "false");
}

void 
Ut_BatteryDbusInterface::testGetPSMAutoValue ()
{
    bool success;

    success = waitforit ("PSMAutoValueArrived", &m_priv->m_PSMAutoValueArrived);
    QVERIFY (success);
    
    qDebug() << "*** PSMAutoValue    = " << 
            (m_priv->m_PSMAutoValue ? "true" : "false");
}

void 
Ut_BatteryDbusInterface::testGetThresholdValue ()
{
    bool success;

    success = waitforit ("*** m_ThresholdValueArrived", 
            &m_priv->m_ThresholdValueArrived);
    QVERIFY (success);

    qDebug() << "m_ThresholdValue = " << m_priv->m_ThresholdValue;
}

void 
Ut_BatteryDbusInterface::testGetThresholdValues ()
{
    bool success;
    int  n = 0;

    success = waitforit ("m_ThresholdValuesArrived", 
            &m_priv->m_ThresholdValuesArrived);
    QVERIFY (success);

    foreach (QString value, m_priv->m_ThresholdValues) {
        qDebug () << "*** m_ThresholdValues [" << n << "] = "<< value;
        ++n;
    }
}

void 
Ut_BatteryDbusInterface::testGetRemainingTimeValues ()
{
    bool success;
    int  n = 0;

    success = waitforit ("m_RemainingTimeValuesArrived", 
            &m_priv->m_RemainingTimeValuesArrived);
    QVERIFY (success);

    foreach (QString value, m_priv->m_RemainingTimeValues) {
        qDebug () << "*** m_RemainingTimeValues [" << n << "] = "<< value;
        ++n;
    }
}

void 
Ut_BatteryDbusInterface::testGetBatteryBarValue ()
{
    bool success;

    success = waitforit ("m_batteryBarValueReceived", 
            &m_priv->m_batteryBarValueReceived);
    QVERIFY (success);

    qDebug() << "*** m_batteryBarValue = " << m_priv->m_batteryBarValue;
}

/*
 * Helper function to wait a boolean to become true using some timeout.
 */
bool 
Ut_BatteryDbusInterface::waitforit (
        const QString &name, 
        bool          *what)
{
    int n = 0;

    for (;;) {
        /*
         * We check if the boolean has been changed to true.
         */
        //qDebug() << "*** " << name << " = " << (*what ? "true" : "false");
        if (*what) 
            return true;
        
        /*
         * If not we wait some time.
         */
        qDebug() << "Waiting " << oneStepWaiting << " uSec for the " << name;
        QTest::qWait (oneStepWaiting);
        ++n;
        
        /*
         * Here is some timeout, we should not wait too much.
         */
        if (n > 10)
            return false;
    }
}

QTEST_APPLESS_MAIN(Ut_BatteryDbusInterface)
