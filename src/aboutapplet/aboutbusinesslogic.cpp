/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of meegotouch-controlpanelapplets.
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
#include "aboutbusinesslogic.h"
#include <QFile>
#include <QTimer>
#include <QSystemInfo>
#include <QNetworkInterface>
#include <QSystemDeviceInfo>
#include <QSystemNetworkInfo>

QTM_USE_NAMESPACE

#include <QRegExp>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QtDBus>
#include <QDBusInterface>
#include <QDBusObjectPath>

#define OS_NAME_FALLBACK "MeeGo"

#undef DEBUG
#define WARNING
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

#ifndef LICENSE_PATH
/* Define an empty string if fallback license file is not defined */
#define LICENSE_PATH ""
#endif

static const QString configPath ("/usr/share/about-contents/");
static const QString configFile (configPath + "contents.ini");

AboutBusinessLogic::AboutBusinessLogic() : QThread (0)
{
    setTerminationEnabled (true);
    qRegisterMetaType<AboutBusinessLogic::requestType>(
        "AboutBusinessLogic::requestType");

    connect (this, SIGNAL (started ()),
             SLOT (initializeAndStart ()));
}

AboutBusinessLogic::~AboutBusinessLogic()
{
    if (m_ManagerDBusIf)
        delete m_ManagerDBusIf;
    if (m_AdapterDBusIf)
        delete m_AdapterDBusIf;
}

void
AboutBusinessLogic::run ()
{
    m_queue.prepend (reqStartAll);
    setPriority (QThread::LowPriority);
}

QString
AboutBusinessLogic::osVersion ()
{
    QString retval;

    QSystemInfo systemInfo;

    retval = systemInfo.version (QSystemInfo::Firmware);
    /*
     * The fw-version format is something like that:
     * SupportedHWID_PROGRAM_ReleaseVersion_suffixes
     *
     * Try to get only the version number:
     */
    int index = retval.indexOf ('_', retval.indexOf ('_') + 1) + 1;
    if (index > 0)
        retval = retval.mid (index);

    if ((retval.isNull () == false) && (retval != ""))
    {
        return retval;
    }

    /*
     * This is a fallback method... (works fine on Ubuntu)
     * Try to get the version number from the lsb-release
     */
    QFile lsbrel_file ("/etc/lsb-release");
    if (lsbrel_file.open (QIODevice::ReadOnly))
    {
        QString contents (lsbrel_file.readAll ().constData ());
        lsbrel_file.close ();

        QRegExp version ("DISTRIB_RELEASE=(\\S*)");
        int pos = version.indexIn (contents);
        if (pos > -1)
        {
            retval = version.cap (1);
            return retval;
        }
    }

    return "-";
}

QString
AboutBusinessLogic::osName ()
{
    //% "MeeGo"
    QString retval = qtTrId ("qtn_prod_sw_version");

    /*
     * Try to get the version number from the lsb-release
     */
    QFile lsbrel_file ("/etc/lsb-release");
    if (lsbrel_file.open (QIODevice::ReadOnly))
    {
        QString contents (lsbrel_file.readAll ().constData ());
        lsbrel_file.close ();

        QRegExp distrib_name ("DISTRIB_ID=(\\S*)");
        int pos = distrib_name.indexIn (contents);
        if (pos > -1)
            retval = distrib_name.cap (1);
    }

    return retval;
}

/*!
 * Returns the MAC address for the first interface it founds. This method
 * contains a static list of interface names that the method will try to find.
 */
QString
AboutBusinessLogic::WiFiAddress ()
{
    QString address;
    QSystemNetworkInfo netInfo;

    address = netInfo.interfaceForMode (
            QSystemNetworkInfo::WlanMode).hardwareAddress ();

    if (address.isNull ())
        address = netInfo.interfaceForMode (
            QSystemNetworkInfo::WimaxMode).hardwareAddress ();

    return address;
}

/*!
 * Returns the IMEI address as a string.
 */
QString
AboutBusinessLogic::IMEI ()
{
    QString imei;

    // Get IMEI from QtMobility SystemDeviceInfo obj.
    QSystemDeviceInfo deviceInfo;

    imei = deviceInfo.imei ();

    return imei;
}

QString
AboutBusinessLogic::productName ()
{
    /*
     * Customization maybe overrides the qt-mobility value...
     */
    if (m_swName.isEmpty ())
        return m_swName;

    QSystemDeviceInfo sdi;
    return sdi.productName ();
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
    QString btAddress;

    btAddress = properties["Address"].toString ();

    SYS_DEBUG ("address = %s", SYS_STR (btAddress));
    delete m_AdapterDBusIf;

    emit requestFinished (reqBtAddr, btAddress);
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

QString
AboutBusinessLogic::licenseText ()
{
    if (m_licenseFile.at (0) != '/')
        m_licenseFile = configPath + m_licenseFile;

    QFile licenseFile (m_licenseFile);
    if (!licenseFile.open (QIODevice::ReadOnly | QIODevice::Text))
    {
        SYS_WARNING ("Unable to open %s", SYS_STR (configPath + m_licenseFile));
        return "";
    }

    QTextStream inStream (&licenseFile);

    return inStream.readAll ();
}

void
AboutBusinessLogic::initializeAndStart ()
{
    /*
     * Load the customization file...
     */
    QSettings content (configFile, QSettings::IniFormat);

    m_swName = content.value ("swname").toString ();
    m_prodName = content.value ("prodname").toString ();
    m_certsImage = content.value ("swtypeimage").toString ();
    m_barcodeImage = content.value ("barcodeimage").toString ();
    m_licenseFile =
        content.value ("abouttext", QString (LICENSE_PATH)).toString ();

    SYS_DEBUG ("\nAbout applet configuration:\n"
               "Product name        : %s\n"
               "Software version    : %s\n"
               "License file        : %s\n"
               "Type approval img.  : %s\n"
               "2D Barcode image    : %s\n",
               SYS_STR (m_swName), SYS_STR (m_prodName),
               SYS_STR (m_licenseFile), SYS_STR (m_certsImage),
               SYS_STR (m_barcodeImage));

    processNextRequest ();
}

QImage
AboutBusinessLogic::certsImage ()
{
    /* Load the picture ... */
    if (m_certsImage.at (0) != '/')
        m_certsImage = configPath + m_certsImage;

    QImage img (m_certsImage);
    SYS_DEBUG ("Image path = %s", SYS_STR (m_certsImage));

    return img;
}

QImage
AboutBusinessLogic::barcodeImage ()
{
    /* Load the picture ... */
    if (m_barcodeImage.at (0) != '/')
        m_barcodeImage = configPath + m_barcodeImage;

    QImage img (m_barcodeImage);
    SYS_DEBUG ("Image path = %s",
               SYS_STR (m_barcodeImage));
    return img;
}

void
AboutBusinessLogic::processNextRequest ()
{
    if (m_queue.isEmpty ())
    {
        return;
    }

    requestType current = m_queue.takeFirst ();
    SYS_DEBUG ("*** current req = %d", (int) current);

    switch (current)
    {
        case reqProdName:
            emit requestFinished (current, productName ());
            break;
        case reqSwName:
            emit requestFinished (current, osName ());
            break;
        case reqLicense:
            emit requestFinished (current, licenseText ());
            break;
        case reqCertsImage:
            emit requestFinished (current, certsImage ());
            break;
        case reqBarcodeImage:
            emit requestFinished (current, barcodeImage ());
            break;
        case reqOsVersion:
            emit requestFinished (current, osVersion ());
            break;
        case reqImei:
            emit requestFinished (current, IMEI ());
            break;
        case reqWifiAddr:
            emit requestFinished (current, WiFiAddress ());
            break;
        case reqBtAddr:
            /* defaultBluetoothAdapterAddressReceived
             * will emit the requestFinished ... */
            initiateBluetoothQueries ();
            break;
        case reqLast:
            /* no-op */
            break;
        default:
        case reqStartAll:
        {
            m_queue.clear ();
            /* Fill the queue with all requests */
            for (int i = (int) reqProdName; i < (int) reqLast; i++)
                m_queue.append ((requestType) i);
        }
        break;
    }

    QTimer::singleShot (0, this, SLOT (processNextRequest ()));
}

