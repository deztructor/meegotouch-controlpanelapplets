/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */

#include "aboutbusinesslogic.h"

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sysinfo.h>

#include <QByteArray>
#include <QString>
#include <QStringList>
#include <QtDBus>
#include <QDBusInterface>
#include <QDBusObjectPath>
#include "phoneinfo.h"

#define OS_NAME_FALLBACK "MeeGo"

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


AboutBusinessLogic::AboutBusinessLogic() :
    m_PhoneInfo (NULL),
    m_gotBluetoothAddress (false),
    m_gotImei (false)
{
}

AboutBusinessLogic::~AboutBusinessLogic()
{
    if (m_ManagerDBusIf)
        delete m_ManagerDBusIf;
    if (m_AdapterDBusIf)
        delete m_AdapterDBusIf;
    if (m_PhoneInfo)
        delete m_PhoneInfo;
}

/*!
 * Starts collecting data.
 */
void
AboutBusinessLogic::initiateDataCollection()
{
    initiateBluetoothQueries ();
    initiatePhoneQueries ();

    osName ();
    osVersion ();
    WiFiAddress ();
    IMEI ();
}

QString 
AboutBusinessLogic::osVersion ()
{
    QString retval = "-";
    const char *key = "/device/sw-release-ver";
    /*
     * INFO: maybe imei number could be fetched in the same
     * way (key for imei : /certs/npc/esn/gsm )
     */

    if (!m_OsVersion.isEmpty())
        return m_OsVersion;


    SYS_DEBUG ("init");
    struct system_config *sc = 0;
    if (sysinfo_init (&sc) == 0)
    {
        uint8_t *data = 0;
        unsigned long size = 0;
        if ((sysinfo_get_value (sc, key, &data, &size) == 0) && (data != 0))
        {
            char *fw_version = new char[size+2];
            qstrncpy (fw_version, (const char *) data, size + 1);
            delete data;

            retval = QString (fw_version);
            delete[] fw_version;
            /* 
             * The fw-version format is something like that:
             * SupportedHWID_PROGRAM_ReleaseVersion_suffixes
             *
             * Try to get only the version number:
             */
            int index = retval.indexOf ('_', retval.indexOf ('_') + 1) + 1;
            if (index > 0)
                retval = retval.mid (index);

            m_OsVersion = retval;
        }
        sysinfo_finish (sc);
    }

    return retval;
}

QString
AboutBusinessLogic::osName ()
{
    //% "MeeGo"
    QString retval = qtTrId ("qtn_prod_sw_version");

    if (!m_OsName.isEmpty())
        return m_OsName;

    /*
     * TODO: Implement something here...
     * [but currently there is no way to get the
     *  software name... ]
     */

    m_OsName = retval;
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

	snprintf (line, 18, "%02X:%02X:%02X:%02X:%02X:%02X",
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

    if (!m_WifiAddress.isEmpty())
        return m_WifiAddress;

    for (int n = 0; interfaceNames[n] != NULL; ++n) {
        retval = WiFiAddress (interfaceNames[n]);
        SYS_DEBUG ("*** %s%s%s address is %s", 
                TERM_YELLOW, interfaceNames[n], TERM_NORMAL, 
                SYS_STR(retval));
        if (!retval.isEmpty())
            break;
    }

    m_WifiAddress = retval;
    return retval;
}

/*!
 * Returns the address of the default bluetooth adapter.
 */
QString 
AboutBusinessLogic::BluetoothAddress ()
{
    SYS_DEBUG ("*** m_gotBluetoothAddress = %s", SYS_BOOL(m_gotBluetoothAddress));
    SYS_DEBUG ("*** m_BluetoothAddress    = %s", SYS_STR(m_BluetoothAddress));
    return m_BluetoothAddress;
}

/*!
 * Returns the IMEI address as a string.
 */
QString 
AboutBusinessLogic::IMEI ()
{
    SYS_DEBUG ("*** m_gotImei             = %s", SYS_BOOL(m_gotImei));
    SYS_DEBUG ("*** m_Imei                = %s", SYS_STR(m_Imei));
    if (m_gotImei) 
        return m_Imei;

    if (!m_PhoneInfo)
        initiatePhoneQueries ();

    m_Imei = m_PhoneInfo->imeiNumber ();
    m_gotImei = true;
    delete m_PhoneInfo;
    m_PhoneInfo = NULL;

    return m_Imei;
}

/*!
 * Initiates a query to receive the default bluetooth adapter name. Later on an
 * other query is going to be initiated to get the address of the default
 * adapter.
 */
void
AboutBusinessLogic::initiateBluetoothQueries ()
{
    if (m_ManagerDBusIf)
        return;

    m_ManagerDBusIf = new QDBusInterface (
            DBUS_BLUEZ_SERVICE, 
            DBUS_BLUEZ_OBJECT_PATH, 
            DBUS_BLUEZ_MANAGER_INTERFACE, 
            QDBusConnection::systemBus ());

    m_ManagerDBusIf->callWithCallback (
            QString (DBUS_BLUEZ_GET_DEFAULT_ADAPTER_METHOD), 
            QList<QVariant> (), this,
            SLOT (defaultBluetoothAdapterReceived(QDBusObjectPath)),
            SLOT (DBusMessagingFailure (QDBusError)));
}

/*!
 * Initiate all the necessary queries through the dbus to get the IMAI address
 * of the phone.
 */
void
AboutBusinessLogic::initiatePhoneQueries ()
{
    m_PhoneInfo = new PhoneInfo;
    SYS_DEBUG ("*** created %p", (void *)m_PhoneInfo);
}

/*!
 * This slot is called when the default adapter name of the bluetooth has been
 * received through the dbus. This method will initiate a new dbus query to get
 * the address of this adapter.
 */
void
AboutBusinessLogic::defaultBluetoothAdapterReceived (
		QDBusObjectPath adapter)
{
    QDBusInterface  *m_AdapterDBusIf;
    
    SYS_DEBUG ("Defaultadapter: %s", SYS_STR (adapter.path()));
    m_AdapterDBusIf = new QDBusInterface (
            DBUS_BLUEZ_SERVICE, 
            adapter.path(),
            DBUS_BLUEZ_ADAPTER_INTERFACE,
            QDBusConnection::systemBus());

    m_AdapterDBusIf->callWithCallback (
            QString (DBUS_BLUEZ_GET_PROPERTIES_METHOD), 
            QList<QVariant>(), this,
            SLOT (defaultBluetoothAdapterAddressReceived(QMap<QString, QVariant>)),
            SLOT (DBusMessagingFailure (QDBusError)));

    delete m_ManagerDBusIf;
}

/*!
 * This slot is called when the address of the default bluetooth adapter has
 * been received.
 */
void
AboutBusinessLogic::defaultBluetoothAdapterAddressReceived (
        QMap<QString, QVariant> properties)
{
    SYS_DEBUG("");
    m_BluetoothAddress = properties["Address"].toString();
    m_gotBluetoothAddress = true;
    SYS_DEBUG ("address = %s", SYS_STR(m_BluetoothAddress));
    delete m_AdapterDBusIf;

    /*
     * Currently only the bluetoot address is handled asynchronously, so if we
     * have it we have them all.
     */
    emit ready();
}

/*!
 * This slot is called when an error is occured during the dbus communication.
 * The error message is printed as a warning message. 
 */
void
AboutBusinessLogic::DBusMessagingFailure (
		QDBusError error)
{
    SYS_WARNING ("%s: %s", SYS_STR (error.name()), SYS_STR (error.message()));
}


