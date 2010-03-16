/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#include "ut_leddbusinterface.h"
#include "leddbusinterface.h"
#include <DuiApplication>
#include <QString>
#include <QStringList>

#define DEBUG
#include "../../src/debug.h"

const int oneStepWaiting = 100;

/******************************************************************************
 * Ut_LedDbusInterfacePrivate implementation.
 */
Ut_LedDbusInterfacePrivate::Ut_LedDbusInterfacePrivate () :
    m_ledStateReceived (false),
    m_illuminationLedStateReceived (false),
    m_eventsLedStateReceived (false)
{
}

void
Ut_LedDbusInterfacePrivate::ledStateReceived (
        bool state)
{
    qDebug() << __func__ << "Signal received";
    m_ledStateReceived = true;
}

void
Ut_LedDbusInterfacePrivate::illuminationLedStateReceived (
        bool state)
{
    qDebug() << __func__ << "Signal received";
    m_illuminationLedStateReceived = true;
}

void
Ut_LedDbusInterfacePrivate::eventsLedStateReceived (
        bool state)
{
    qDebug() << __func__ << "Signal received";
    m_eventsLedStateReceived = true;
}


/******************************************************************************
 * Ut_LedDbusInterface implementation. 
 */
void 
Ut_LedDbusInterface::init()
{
}

void 
Ut_LedDbusInterface::cleanup()
{
}

DuiApplication *app;


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
    QTest::qWait (5000);
    
    #if 1
    qDebug() << "--- What about the sysuid? Is it running? ------";
    system ("ps axu | grep sysuid | grep -v grep");
    qDebug() << "------------------------------------------------";
    #endif
}

void 
Ut_LedDbusInterface::initTestCase()
{

    int argc = 1;
    char* app_name = (char*) "./Ut_LedDbusInterface";
    app = new DuiApplication(argc, &app_name);

    waitForSysuidRunning();

    m_priv = new Ut_LedDbusInterfacePrivate;
    m_LedDBusInterface = new LedDBusInterface;

    connect (m_LedDBusInterface, SIGNAL(ledStateReceived(bool)),
            m_priv, SLOT(ledStateReceived(bool)));
    connect (m_LedDBusInterface, SIGNAL(illuminationLedStateReceived(bool)),
            m_priv, SLOT(illuminationLedStateReceived(bool)));
    connect (m_LedDBusInterface, SIGNAL(eventsLedStateReceived(bool)),
            m_priv, SLOT(eventsLedStateReceived(bool)));

    m_LedDBusInterface->ledStateRequired ();
    m_LedDBusInterface->illuminationLedStateRequired ();
    m_LedDBusInterface->eventsLedStateRequired ();
}

void 
Ut_LedDbusInterface::cleanupTestCase()
{
    delete m_priv;
    m_priv = 0;

    delete m_LedDBusInterface;
    m_LedDBusInterface = 0;

    delete app;
}


void
Ut_LedDbusInterface::testLedStatesArrived ()
{
    bool success;

    success = waitforit ("m_ledStateReceived", 
            &m_priv->m_ledStateReceived);
    QVERIFY (success);
    
    success = waitforit ("m_illuminationLedStateReceived", 
            &m_priv->m_illuminationLedStateReceived);
    QVERIFY (success);
    
    success = waitforit ("m_eventsLedStateReceived", 
            &m_priv->m_eventsLedStateReceived);
    QVERIFY (success);
}

/*
 * Helper function to wait a boolean to become true using some timeout.
 */
bool 
Ut_LedDbusInterface::waitforit (
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

QTEST_APPLESS_MAIN(Ut_LedDbusInterface)

