/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#include "ft_aboutbusinesslogic.h"
#include "aboutbusinesslogic.h"

#include <MApplication>
#include <QList>

#define DEBUG
#include "../../src/debug.h"

/******************************************************************************
 * SignalSink implementation.
 */
SignalSink::SignalSink () : 
    m_ReadyCame (false)
{
    
}

void
SignalSink::ready ()
{
    m_ReadyCame = true;
}

/******************************************************************************
 * Ft_AboutBusinessLogic implementation. 
 */
void Ft_AboutBusinessLogic::init()
{
}

void 
Ft_AboutBusinessLogic::cleanup()
{
}


static int argc = 1;
static char* app_name = (char*) "./Ft_AboutBusinessLogic";

void 
Ft_AboutBusinessLogic::initTestCase()
{
    bool connectSuccess;

    m_App = new MApplication (argc, &app_name);
    m_Api = new AboutBusinessLogic;

    connectSuccess = connect (
            m_Api, SIGNAL(ready()),
            &m_SignalSink, SLOT(ready()));
    QVERIFY(connectSuccess);
}

void 
Ft_AboutBusinessLogic::cleanupTestCase()
{
    delete m_App;
    delete m_Api;
}

void
Ft_AboutBusinessLogic::testCollectData ()
{
    /*
     * Can't make this test running inside scratchbox. 
     */
    #ifdef __i386__
    return;
    #endif

    /*
     * Initiating the data collection and waiting for some time the
     * businesslogic to collect all the data.
     */
    m_Api->initiateDataCollection ();
    QTest::qWait (1000);

    /*
     * Checking if the data collection is ready and a signal is sent.
     */
    if (!m_SignalSink.m_ReadyCame) {
        SYS_WARNING (
"The AboutBusinessLogic did not collect the data. When the bluetoothd is not "
"running this is what happens. So: does the bluetoothd running?");
        system ("ps axu | grep bluetoothd");
    }
    QVERIFY (m_SignalSink.m_ReadyCame);

    /*
     * Reading all the information the businesslogic was able to collect.
     */
    QString osVersion   = m_Api->osVersion ();
    QString osName      = m_Api->osName ();
    QString WiFiAddress = m_Api->WiFiAddress ();
    QString BluetoothAddress = m_Api->BluetoothAddress ();
    QString IMEI        = m_Api->IMEI ();
    
    SYS_DEBUG ("*** osVersion()   = %s", SYS_STR(osVersion));
    SYS_DEBUG ("*** osName()      = %s", SYS_STR(osName));
    SYS_DEBUG ("*** WiFiAddress() = %s", SYS_STR(WiFiAddress));
    SYS_DEBUG ("*** BluetoothAddress = %s", SYS_STR(BluetoothAddress));
    SYS_DEBUG ("*** IMEI          = %s", SYS_STR(IMEI));

    /*
     * These are not available under scratchbox.
     */
    QVERIFY (!osVersion.isEmpty());
    QVERIFY (!IMEI.isEmpty());
    
    /*
     * But these are!
     */
    QVERIFY (!osName.isEmpty());
    QVERIFY (!WiFiAddress.isEmpty());
    QVERIFY (!BluetoothAddress.isEmpty());
}


QTEST_APPLESS_MAIN(Ft_AboutBusinessLogic)
