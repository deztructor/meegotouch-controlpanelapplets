/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */
#include "ut_aboutbusinesslogic.h"
#include "mdesktopentry.h"
#include "aboutbusinesslogic.h"

#include "phoneinfo.h"
#ifndef PHONEINFO_STUBBED_H
#  error "PhoneInfo is not stubbed, can't continue."
#endif

#include <stdint.h>
#include <inttypes.h>
#include <QMap>
#include <QString>
#include <QVariant>
#include <QByteArray>

#include <MApplication>

#define DEBUG
#include "../../src/debug.h"

#if 0
/******************************************************************************
 * Stubbing QFile.
 */
#include <QFile>

bool fileOpenSuccess = true;

bool
QFile::open (
        OpenMode mode)
{
    SYS_DEBUG ("*** filename = %s", SYS_STR(fileName()));
    if (fileName() == "/tmp/osso-product-info")
        return fileOpenSuccess;

    return false;
}

static const char *fileContent = NULL;

qint64
QIODevice::readLine (
        char     *data, 
        qint64    maxSize)
{
    qint64 retval = -1;

    if (fileContent != NULL) {
        strcpy (data, fileContent);
        //SYS_DEBUG ("*** data = %s", data);
        fileContent = NULL;
        retval = strlen (data);
    }

    //SYS_DEBUG ("*** maxSize = %lld", maxSize);
    //SYS_DEBUG ("*** retval  = %lld", retval);
    return retval;
}
#endif
/******************************************************************************
 * Stubbing the sysinfo
 */
extern "C" {

int
sysinfo_init (struct system_config **sc_out)
{
    Q_UNUSED (sc_out);
    return 0;
}

void
sysinfo_finish (struct system_config *sc)
{
    Q_UNUSED (sc);
}

char *sysinfo_get_value_retval = 0;

int
sysinfo_get_value (struct system_config *sc, const char *key,
                   uint8_t **val_out, unsigned long *len_out)
{
    Q_UNUSED (sc);

    SYS_DEBUG ("key = %s", key);
    *val_out = (uint8_t *) qstrdup (sysinfo_get_value_retval);
    *len_out = (unsigned long) qstrlen (sysinfo_get_value_retval);

    return 0;
}

};

/******************************************************************************
 * Stubbing the dbus interface.
 */
static QString lastCalledMethod;

bool
QDBusAbstractInterface::callWithCallback (
        const QString           &method, 
        const QList<QVariant>   &args, 
        QObject                 *receiver, 
        const char              *returnMethod, 
        const char              *errorMethod)
{
    Q_UNUSED (args);
    Q_UNUSED (receiver);
    Q_UNUSED (errorMethod);

    SYS_DEBUG ("*** method       = %s", SYS_STR(method));
    SYS_DEBUG ("*** returnMethod = %s", returnMethod);

    lastCalledMethod = method;
    return true;
}

/******************************************************************************
 * Ut_AboutBusinessLogic implementation. 
 */
void 
Ut_AboutBusinessLogic::init()
{
}

void 
Ut_AboutBusinessLogic::cleanup()
{
}


static int argc = 1;
static char *app_name = (char*) "./Ut_AboutBusinessLogic";

void 
Ut_AboutBusinessLogic::initTestCase()
{
    m_App = new MApplication (argc, &app_name);
    m_Api = 0;
}

void 
Ut_AboutBusinessLogic::cleanupTestCase()
{
    if (m_Api)
        delete m_Api;
    
    delete m_App;
}

void 
Ut_AboutBusinessLogic::testImei()
{
    QString imei;

    m_Api = new AboutBusinessLogic;
    imei = m_Api->IMEI();
    QVERIFY (imei == FAKE_IMEI_NUMBER);
    SYS_DEBUG ("*** IMEI = %s", SYS_STR(imei));
    // Repeating
    imei = m_Api->IMEI();
    QVERIFY (imei == FAKE_IMEI_NUMBER);
    SYS_DEBUG ("*** IMEI = %s", SYS_STR(imei));

    delete m_Api;
}

void 
Ut_AboutBusinessLogic::testOsName ()
{
    QString name;

    m_Api = new AboutBusinessLogic;

#if 0
    /*
     * Simulating unsuccessfull file open.
     */
    fileOpenSuccess = false;
    name = m_Api->osName();
    SYS_DEBUG ("*** osName (nofile) = %s", SYS_STR(name));
    QVERIFY (name == "qtn_prod_sw_version");
    delete m_Api;

    /*
     * Simulating empty file.
     */
    m_Api = new AboutBusinessLogic;
    fileOpenSuccess = true;
    name = m_Api->osName();
    SYS_DEBUG ("*** osName (emptyfile) = %s", SYS_STR(name));
    QVERIFY (name == "qtn_prod_sw_version");
    delete m_Api;

    /*
     * Testing with simulating a the perfect input file.
     */
    m_Api = new AboutBusinessLogic;
    fileOpenSuccess = true;
    fileContent = "OSSO_PRODUCT_RELEASE_NAME='fakeOsName'\n";
    name = m_Api->osName();
    SYS_DEBUG ("*** osName (rightfile) = %s", SYS_STR(name));
    QVERIFY (name == "fakeOsName");
    // Repeating with the cached version.
    name = m_Api->osName();
    SYS_DEBUG ("*** osName (rightfile) = %s", SYS_STR(name));
    QVERIFY (name == "fakeOsName");
#endif

    name = m_Api->osName();
    QCOMPARE (name, QString ("qtn_prod_sw_version"));
    delete m_Api;
}

void 
Ut_AboutBusinessLogic::testOsVersion ()
{
    QString name;

    m_Api = new AboutBusinessLogic;

#if 0
    /*
     * Simulating unsuccessfull file open.
     */
    fileOpenSuccess = false;
    name = m_Api->osVersion();
    SYS_DEBUG ("*** osVersion (nofile) = %s", SYS_STR(name));
    //QVERIFY (name == "qtn_prod_sw_version");
    delete m_Api;

    /*
     * Simulating empty file.
     */
    m_Api = new AboutBusinessLogic;
    fileOpenSuccess = true;
    name = m_Api->osVersion();
    SYS_DEBUG ("*** osVersion (emptyfile) = %s", SYS_STR(name));
    //QVERIFY (name == "qtn_prod_sw_version");
    delete m_Api;

    /*
     * Testing with simulating a the perfect input file.
     */
    m_Api = new AboutBusinessLogic;
    fileOpenSuccess = true;
    fileContent = "OSSO_PRODUCT_RELEASE_VERSION='fakeOsVersion'\n";
    name = m_Api->osVersion();
    SYS_DEBUG ("*** osName (rightfile) = %s", SYS_STR(name));
    QVERIFY (name == "fakeOsVersion");

    // Repeating with the cached version.
    name = m_Api->osVersion();
    SYS_DEBUG ("*** osName (rightfile) = %s", SYS_STR(name));
    QVERIFY (name == "fakeOsVersion");
#endif

    sysinfo_get_value_retval = (char *) "HARDWARE_PROGRAM_VERSION13";
    name = m_Api->osVersion();

    QCOMPARE (name, QString ("VERSION13"));

    delete m_Api;
}


void 
Ut_AboutBusinessLogic::testBluetooth ()
{
    QMap <QString, QVariant> properties;
    QString                  address;

    properties["Address"] = QVariant(QString("fake-bluetooth-address"));
    m_Api = new AboutBusinessLogic; 
    
    /*
     * Let's initiate the Bluetooth query that will call the
     * QDBusAbstractInterface::callWithCallback() that is stubbed.
     * FIXME: Once we stub the socket, the ioctl and the close() we can call
     * initiateDataCollection() to achieve a better coverage.
     */
    //m_Api->initiateDataCollection();
    m_Api->initiateBluetoothQueries ();
    QVERIFY (lastCalledMethod == "DefaultAdapter");

    /*
     * Let's answer the previous step by calling the DBus callback manually.
     * This will initiate an other DBus call also stubbed.
     */
    m_Api->defaultBluetoothAdapterReceived (
            QDBusObjectPath("/fakeObjectPath"));
    QVERIFY (lastCalledMethod == "GetProperties");

    /*
     * Answering the second DBus call and checking if the businesslogic
     * processed the data as it should.
     */
    m_Api->defaultBluetoothAdapterAddressReceived (properties);
    address = m_Api->BluetoothAddress ();
    SYS_DEBUG ("address = %s", SYS_STR(address));
    QVERIFY (address == "fake-bluetooth-address");

    /*
     * Let's test the failure socket. This does not do nothing...
     */
    m_Api->DBusMessagingFailure (QDBusError());

    delete m_Api;

    /*
     * Let's see what happens if we initate the data collection and instead of
     * producing answers to queries we just destroy the object.
     */
    m_Api = new AboutBusinessLogic; 
    m_Api->initiatePhoneQueries ();
    m_Api->initiateBluetoothQueries ();
    delete m_Api;
}

QTEST_APPLESS_MAIN(Ut_AboutBusinessLogic)

