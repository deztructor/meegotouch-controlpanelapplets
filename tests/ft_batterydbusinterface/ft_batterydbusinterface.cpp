/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#include "ft_batterydbusinterface.h"
#include "batterydbusinterface.h"
#include <MApplication>
#include <QString>
#include <QStringList>

#define DEBUG
#include "../../src/debug.h"

const int oneStepWaiting = 100;

/******************************************************************************
 * Ft_BatteryDbusInterfacePrivate implementation.
 */
Ft_BatteryDbusInterfacePrivate::Ft_BatteryDbusInterfacePrivate () :
    m_PSMValueArrived (false),
    m_PSMValue (false),
    m_PSMAutoValueArrived (false),
    m_PSMAutoValue (false),
    m_ThresholdValueArrived (false),
    m_ThresholdValuesArrived (false),
    m_RemainingTimeValuesArrived (false),
    m_batteryBarValueReceived (0),
    m_batteryBarValue (0)
{
}

void
Ft_BatteryDbusInterfacePrivate::PSMValueReceived (
        bool PSMValue)
{
    m_PSMValueArrived = true;
    m_PSMValue = PSMValue;
}

void
Ft_BatteryDbusInterfacePrivate::PSMAutoValueReceived (
        bool PSMAutoValue)
{
    m_PSMAutoValueArrived = true;
    m_PSMAutoValue = PSMAutoValue;
}

void
Ft_BatteryDbusInterfacePrivate::PSMThresholdValueReceived (
        QString ThresholdValue)
{
    m_ThresholdValueArrived = true;
    m_ThresholdValue = ThresholdValue;
}

void
Ft_BatteryDbusInterfacePrivate::PSMThresholdValuesReceived (
        QStringList ThresholdValues)
{
    m_ThresholdValuesArrived = true;
    m_ThresholdValues = ThresholdValues;
}

void
Ft_BatteryDbusInterfacePrivate::remainingTimeValuesReceived (
        QStringList RemainingTimeValues)
{
    m_RemainingTimeValuesArrived = true;
    m_RemainingTimeValues = RemainingTimeValues;
}

void
Ft_BatteryDbusInterfacePrivate::batteryBarValueReceived (
        int batteryBarValue)
{
    m_batteryBarValueReceived = true;
    m_batteryBarValue = batteryBarValue;
}

/******************************************************************************
 * Ft_BatteryDbusInterface implementation. 
 */
void 
Ft_BatteryDbusInterface::init()
{
}

void 
Ft_BatteryDbusInterface::cleanup()
{
}

MApplication *app;


static bool
sysuid_running ()
{
    FILE *pipe;
    char  line[256];
    int   length;

    pipe = popen ("ps axu | grep sysuid | grep -v ps | grep -v grep", "r");
    if (pipe == NULL) {
        qDebug() << "popen() failed";
        return false;
    }

    if (fgets(line, 255, pipe) == NULL) {
        //qDebug() << "fgets() failed";
        pclose (pipe);
        return false;
    }

    qDebug() << "line is " << line;
    pclose (pipe);

    line[255] = '\0';

    length = strlen (line);
    qDebug() << "length = " << length;
    if (length != 0)
        return true;

    return false;
}

static void 
waitForSysuidRunning ()
{
    int n = 0;

    while (!sysuid_running()) {
        if (n >= 25)
            break;

        qDebug() << n << "sysuid is not running, waiting 1 sec...";
        
        QTest::qWait (1000);
        ++n;
    }
    /*
     * If the sysuid started we still need to wait a few seconds until it
     * initializes itself.
     */
    QTest::qWait (10000);
    
    #if 1
    qDebug() << "--- What about the sysuid? Is it running? ------";
    system ("ps axu | grep sysuid | grep -v grep");
    qDebug() << "------------------------------------------------";
    #endif
}

static int argc = 1;
static char* app_name = (char*) "./Ft_BatteryDbusInterface";

void 
Ft_BatteryDbusInterface::initTestCase()
{
    app = new MApplication(argc, &app_name);

    waitForSysuidRunning();

    m_priv = new Ft_BatteryDbusInterfacePrivate;
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
Ft_BatteryDbusInterface::cleanupTestCase()
{
    delete m_priv;
    m_priv = 0;

    delete m_BatteryDBusInterface;
    m_BatteryDBusInterface = 0;

    delete app;
}




void 
Ft_BatteryDbusInterface::testGetPSMValue ()
{
    bool success;

    success = waitforit ("PSMValueArrived", &m_priv->m_PSMValueArrived);
    QVERIFY (success);
    
    qDebug() << "*** PSMValue    = " << 
            (m_priv->m_PSMValue ? "true" : "false");
}

void 
Ft_BatteryDbusInterface::testGetPSMAutoValue ()
{
    bool success;

    success = waitforit ("PSMAutoValueArrived", &m_priv->m_PSMAutoValueArrived);
    QVERIFY (success);
    
    qDebug() << "*** PSMAutoValue    = " << 
            (m_priv->m_PSMAutoValue ? "true" : "false");
}

void 
Ft_BatteryDbusInterface::testGetThresholdValue ()
{
    bool success;

    success = waitforit ("*** m_ThresholdValueArrived", 
            &m_priv->m_ThresholdValueArrived);
    QVERIFY (success);

    qDebug() << "m_ThresholdValue = " << m_priv->m_ThresholdValue;
}

void 
Ft_BatteryDbusInterface::testGetThresholdValues ()
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
Ft_BatteryDbusInterface::testGetRemainingTimeValues ()
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
Ft_BatteryDbusInterface::testGetBatteryBarValue ()
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
Ft_BatteryDbusInterface::waitforit (
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

QTEST_APPLESS_MAIN(Ft_BatteryDbusInterface)
