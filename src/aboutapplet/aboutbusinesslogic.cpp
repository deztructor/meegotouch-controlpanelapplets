/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "aboutbusinesslogic.h"

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <QString>
#include <QStringList>
#include <QtDBus>
#include <QFile>

#define DEBUG
#include "../debug.h"

/*
 * Values used for getting bluetooth address by dbus 
 */
#define DBUS_BLUEZ_SERVICE "org.bluez"
#define DBUS_BLUEZ_OBJECT_PATH "/"
#define DBUS_BLUEZ_MANAGER_INTERFACE "org.bluez.Manager"
#define DBUS_BLUEZ_ADAPTER_INTERFACE "org.bluez.Adapter"
#define DBUS_BLUEZ_GET_DEFAULT_ADAPTER_METHOD "DefaultAdapter"
#define DBUS_BLUEZ_GET_PROPERTIES_METHOD "GetProperties"

#define DBUS_SIM_SERVICE "com.nokia.phone.SIM"
#define DBUS_SIM_SECURITY_OBJECT_PATH "/com/nokia/phone/SIM/security"
#define DBUS_SIM_SECURITY_INTERFACE "Phone.Sim.Security"
#define DBUS_SIM_SECURITY_GET_IMEI_METHOD "get_imei"

AboutBusinessLogic::AboutBusinessLogic() :
    m_gotBluetoothAddress (false),
    m_gotImei (false)
{
    initiateBluetoothQueries ();
    initiatePhoneQueries ();
}

AboutBusinessLogic::~AboutBusinessLogic()
{
}

/*!
 * Returns the version number of the operating system (Maemo version).
-16-3:~# cat /tmp/osso-product-info 
or 
-16-3:~# cat /tmp/osso-product-info
OSSO_PRODUCT_HARDWARE='<unknown>'
OSSO_PRODUCT_NAME='<unknown>'
OSSO_PRODUCT_FULL_NAME='<unknown>'
OSSO_PRODUCT_RELEASE_NAME='<unknown>'
OSSO_PRODUCT_RELEASE_FULL_NAME='<unknown>'
OSSO_PRODUCT_RELEASE_VERSION='0.2010.16-3.REQ.243168'
OSSO_PRODUCT_WLAN_CHANNEL='<unknown>'
OSSO_PRODUCT_KEYBOARD='<unknown>'
OSSO_PRODUCT_REGION='<unknown>'
OSSO_PRODUCT_SHORT_NAME='<unknown>'
OSSO_VERSION='RX-71+RM-581+RM-660+RM-680+RM-696_HARMATTAN_0.2010.16-3.REQ.243168_PR_MR0'
 */
QString 
AboutBusinessLogic::osVersion ()
{
    QFile   file("/tmp/osso-product-info");
    QString retval;
    char    buf[1024];
    QString line;
    qint64  lineLength;

    if (!file.open(QFile::ReadOnly)) 
        return retval;

    for (;;) {
        lineLength = file.readLine(buf, sizeof(buf));
        if (lineLength < 0) 
            break;
        line = buf;
        if (!line.startsWith ("OSSO_PRODUCT_RELEASE_VERSION='"))
            continue;
        
        line.remove (0, 30);
        line.chop (2);
        retval = line;
        SYS_DEBUG ("*** line = %s", SYS_STR(line));
    }

    return retval;
}

/*! 
 * Returns the MAC address for the interface with the given name, or returns an
 * empty string if the interface was not found.
 */
QString 
AboutBusinessLogic::WiFiAddress (
        const char *iface)
{
    int           dd;
	struct ifreq  sIfReq;
	unsigned char cMacAddr[8];
    char          line[32];

	for (dd = 0; dd < 8; dd++) 
        cMacAddr[dd] = 0;

	dd = socket (PF_INET, SOCK_STREAM, 0);
	
	if (dd < 0)
		return QString();

	snprintf (sIfReq.ifr_name, IFNAMSIZ, "%s", iface);

	if (ioctl (dd, SIOCGIFHWADDR, &sIfReq) != 0)
		return QString();

	memmove ((void *) &cMacAddr[0], 
            (void *) &sIfReq.ifr_ifru.ifru_hwaddr.sa_data[0], 
            6);

	close (dd);

	sprintf(line, "%02X:%02X:%02X:%02X:%02X:%02X",
            cMacAddr[0], cMacAddr[1], cMacAddr[2], 
            cMacAddr[3], cMacAddr[4], cMacAddr[5]);

    return line;
}

/*!
 * Returns the MAC address for the first interface it founds. This method
 * contains a static list of interface names that the method will try to find.
 */
QString 
AboutBusinessLogic::WiFiAddress ()
{
    QString        retval;
    const char    *interfaceNames[] = {
        "wlan0", "wimax0", NULL
    };

    for (int n = 0; interfaceNames[n] != NULL; ++n) {
        retval = WiFiAddress (interfaceNames[n]);
        SYS_DEBUG ("*** %s12%s%s address is %s", 
                TERM_YELLOW, interfaceNames[n], TERM_NORMAL, 
                SYS_STR(retval));
        if (!retval.isEmpty())
            return retval;
    }

    return retval;
}

QString 
AboutBusinessLogic::BluetoothAddress ()
{
    SYS_DEBUG ("*** m_gotBluetoothAddress = %s", SYS_BOOL(m_gotBluetoothAddress));
    SYS_DEBUG ("*** m_BluetoothAddress    = %s", SYS_STR(m_BluetoothAddress));
    return m_BluetoothAddress;
}

QString 
AboutBusinessLogic::IMEI ()
{
    SYS_DEBUG ("*** m_gotImei             = %s", SYS_BOOL(m_gotImei));
    SYS_DEBUG ("*** m_Imei                = %s", SYS_STR(m_Imei));
    return m_Imei;
}

void
AboutBusinessLogic::initiateBluetoothQueries ()
{
    QDBusInterface  *managerDBusIf;

    managerDBusIf = new QDBusInterface (
            DBUS_BLUEZ_SERVICE, 
            DBUS_BLUEZ_OBJECT_PATH, 
            DBUS_BLUEZ_MANAGER_INTERFACE, 
            QDBusConnection::systemBus ());

    managerDBusIf->callWithCallback (
            QString (DBUS_BLUEZ_GET_DEFAULT_ADAPTER_METHOD), 
            QList<QVariant> (), this,
            SLOT (defaultBluetoothAdapterReceived(QDBusObjectPath)),
            SLOT (DBusMessagingFailure (QDBusError)));
}

void
AboutBusinessLogic::initiatePhoneQueries ()
{
    QDBusInterface  *imeiDBusIf;

    imeiDBusIf = new QDBusInterface (
            DBUS_SIM_SERVICE, 
            DBUS_SIM_SECURITY_OBJECT_PATH, 
            DBUS_SIM_SECURITY_INTERFACE, 
            QDBusConnection::systemBus ());

    imeiDBusIf->callWithCallback (
            QString (DBUS_SIM_SECURITY_GET_IMEI_METHOD), 
            QList<QVariant> (), this,
            SLOT (imeiReceived(QString)),
            SLOT (DBusMessagingFailure (QDBusError)));
}


void
AboutBusinessLogic::defaultBluetoothAdapterReceived (
		QDBusObjectPath adapter)
{
    QDBusInterface  *adapterDBusIf;
    
    SYS_DEBUG ("Defaultadapter: %s", SYS_STR (adapter.path()));
    adapterDBusIf = new QDBusInterface (
            DBUS_BLUEZ_SERVICE, 
            adapter.path(),
            DBUS_BLUEZ_ADAPTER_INTERFACE,
            QDBusConnection::systemBus());

    adapterDBusIf->callWithCallback (
            QString (DBUS_BLUEZ_GET_PROPERTIES_METHOD), 
            QList<QVariant>(), this,
            SLOT (defaultBluetoothAdapterAddressReceived(QMap<QString, QVariant>)),
            SLOT (DBusMessagingFailure (QDBusError)));
}

void
AboutBusinessLogic::imeiReceived (
        QString imei)
{
    m_gotImei = true;
    m_Imei = imei;
}

void
AboutBusinessLogic::defaultBluetoothAdapterAddressReceived (
        QMap<QString, QVariant> properties)
{
    SYS_DEBUG("");
    m_BluetoothAddress = properties["Address"].toString();
    m_gotBluetoothAddress = true;
    SYS_DEBUG ("address = %s", SYS_STR(m_BluetoothAddress));
}

void
AboutBusinessLogic::DBusMessagingFailure (
		QDBusError error)
{
    SYS_WARNING ("%s: %s", SYS_STR (error.name()), SYS_STR (error.message()));
}
