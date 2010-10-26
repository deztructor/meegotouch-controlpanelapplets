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

#ifndef ABOUTBUSINESSLOGIC_H
#define ABOUTBUSINESSLOGIC_H

#include <QPointer>
#include <QObject>
#include <QMap>
#include <QDBusError>
#include <QDBusInterface>

class QDBusObjectPath;
class QString;

/*!
 * This class allows to get some information about system
 */
class AboutBusinessLogic : public QObject
{
    Q_OBJECT

public:
    AboutBusinessLogic ();
    ~AboutBusinessLogic ();

    QString osName ();
    QString osVersion ();
    QString WiFiAddress ();
    QString BluetoothAddress ();
    QString IMEI ();

signals:
    void refreshNeeded ();

public slots:
    void defaultBluetoothAdapterAddressReceived (
        QMap<QString, QVariant> properties);
    void defaultBluetoothAdapterReceived (QDBusObjectPath adapter);
    void DBusMessagingFailure (QDBusError error);

private:
    void initiateBluetoothQueries ();

private:
    QPointer<QDBusInterface> m_ManagerDBusIf;
    QPointer<QDBusInterface> m_AdapterDBusIf;

    QString       m_BluetoothAddress;
    QString       m_Imei;
    QString       m_WifiAddress;
    QString       m_OsName;
    QString       m_OsVersion;
    #ifdef UNIT_TEST
    friend class Ut_AboutBusinessLogic;
    friend class Ut_AboutApplet;
    #endif
};

#endif
