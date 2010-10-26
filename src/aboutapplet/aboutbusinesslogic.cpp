/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
#include <QSystemInfo>
#include <QNetworkInterface>
#include <QSystemDeviceInfo>
#include <QSystemNetworkInfo>

QTM_USE_NAMESPACE

#include <QRegExp>
#include <QString>
#include <QStringList>
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

AboutBusinessLogic::AboutBusinessLogic()
{
}

AboutBusinessLogic::~AboutBusinessLogic()
{
    if (m_ManagerDBusIf)
        delete m_ManagerDBusIf;
    if (m_AdapterDBusIf)
        delete m_AdapterDBusIf;
}

QString
AboutBusinessLogic::osVersion ()
{
    QString retval = "-";

    if (!m_OsVersion.isEmpty())
        return m_OsVersion;

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

    if (retval.isNull () == false)
    {
      m_OsVersion = retval;
      return m_OsVersion;
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
            m_OsVersion = retval;
        }
    }

    return m_OsVersion;
}

QString
AboutBusinessLogic::osName ()
{
    //% "MeeGo"
    QString retval = qtTrId ("qtn_prod_sw_version");

    if (!m_OsName.isEmpty())
        return m_OsName;

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

    m_OsName = retval;
    return retval;
}

/*!
 * Returns the MAC address for the first interface it founds. This method
 * contains a static list of interface names that the method will try to find.
 */
QString
AboutBusinessLogic::WiFiAddress ()
{
    if (!m_WifiAddress.isEmpty())
        return m_WifiAddress;

    QSystemNetworkInfo netInfo;

    m_WifiAddress = netInfo.interfaceForMode (
            QSystemNetworkInfo::WlanMode).hardwareAddress ();

    if (m_WifiAddress.isNull ())
        m_WifiAddress = netInfo.interfaceForMode (
            QSystemNetworkInfo::WimaxMode).hardwareAddress ();

    return m_WifiAddress;
}

/*!
 * Returns the address of the default bluetooth adapter.
 */
QString
AboutBusinessLogic::BluetoothAddress ()
{
    if (m_BluetoothAddress.isNull () == false)
        return m_BluetoothAddress;

    QSystemNetworkInfo netInfo;
    QNetworkInterface bluetooth
        = netInfo.interfaceForMode (QSystemNetworkInfo::BluetoothMode);

    /*
     * Currently the bluetooth hardware address only available from
     * QNetworkInterface when the interface is loaded & up
     */
    if (bluetooth.isValid ())
        m_BluetoothAddress = bluetooth.hardwareAddress ();
    else
        initiateBluetoothQueries ();

    return m_BluetoothAddress;
}

/*!
 * Returns the IMEI address as a string.
 */
QString
AboutBusinessLogic::IMEI ()
{
    if (m_Imei.isNull () == false)
        return m_Imei;

    // Get IMEI from QtMobility SystemDeviceInfo obj.
    QSystemDeviceInfo deviceInfo;

    m_Imei = deviceInfo.imei ();

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
    SYS_DEBUG ("address = %s", SYS_STR(m_BluetoothAddress));
    delete m_AdapterDBusIf;

    /*
     * Currently only the bluetoot address is handled asynchronously, so if we
     * have it we have them all.
     */
    emit refreshNeeded ();
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

