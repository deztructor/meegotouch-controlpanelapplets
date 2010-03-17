/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "batterydbusinterface.h"

#include <QtDBus>
#include <QDebug>
#include <QVariant>

#undef DEBUG
#include "../debug.h"

BatteryDBusInterface::BatteryDBusInterface ()
{
    m_DbusIf = new QDBusInterface (
            "com.nokia.systemui", 
            "/", 
            "com.nokia.systemui.battery", 
            QDBusConnection::sessionBus ());

    connect (m_DbusIf, SIGNAL (batteryCharging (int)),
             this, SIGNAL (batteryCharging (int)));
    connect (m_DbusIf, SIGNAL (batteryNotCharging ()),
             this, SIGNAL (batteryNotCharging()));
    connect (m_DbusIf, SIGNAL (batteryBarValueChanged (int)),
             this, SIGNAL (batteryBarValueReceived (int)));
    connect (m_DbusIf, SIGNAL (PSMValueChanged (bool)),
             this, SIGNAL (PSMValueReceived (bool)));
    connect (m_DbusIf, SIGNAL (PSMAutoDisabled ()),
             this, SIGNAL (PSMAutoDisabled ()));
    connect (m_DbusIf, SIGNAL (remainingTimeValuesChanged (QStringList)),
             this, SIGNAL (remainingTimeValuesReceived (QStringList)));
    connect (m_DbusIf, SIGNAL (batteryFull ()),
             this, SIGNAL (batteryFullyCharged ()));
}

BatteryDBusInterface::~BatteryDBusInterface ()
{
    delete m_DbusIf;
    m_DbusIf = NULL;
}

void
BatteryDBusInterface::PSMValueRequired ()
{
    SYS_DEBUG ("");
    m_DbusIf->callWithCallback (
            QString ("PSMValue"), QList<QVariant> (), this,
            SIGNAL (PSMValueReceived (bool)),
            SLOT (DBusMessagingFailure (QDBusError)));
}

void
BatteryDBusInterface::PSMAutoValueRequired ()
{
    SYS_DEBUG ("");
    
    m_DbusIf->callWithCallback (
            QString ("PSMAutoValue"), QList<QVariant> (), this,
            SIGNAL (PSMAutoValueReceived (bool)),
            SLOT (DBusMessagingFailure (QDBusError)));
}

void
BatteryDBusInterface::batteryChargingStateRequired ()
{
    SYS_DEBUG ("");
    
    m_DbusIf->call (QDBus::NoBlock, QString ("batteryChargingState"));
}

void
BatteryDBusInterface::PSMThresholdValuesRequired ()
{
    SYS_DEBUG ("");
    
    m_DbusIf->callWithCallback (
            QString ("PSMThresholdValues"), QList<QVariant> (), this,
            SIGNAL (PSMThresholdValuesReceived (QStringList)),
            SLOT (DBusMessagingFailure (QDBusError)));
}

void BatteryDBusInterface::PSMThresholdValueRequired()
{
    SYS_DEBUG ("");

    m_DbusIf->callWithCallback (
            QString ("PSMThresholdValue"), QList<QVariant> (), this,
            SIGNAL (PSMThresholdValueReceived (QString)),
            SLOT (DBusMessagingFailure (QDBusError)));
}

void
BatteryDBusInterface::remainingTimeValuesRequired ()
{
    SYS_DEBUG ("");

    m_DbusIf->callWithCallback (
            QString ("remainingTimeValues"), QList<QVariant> (), this,
            SIGNAL (remainingTimeValuesReceived (QStringList)),
            SLOT (DBusMessagingFailure (QDBusError)));
}

void
BatteryDBusInterface::batteryBarValueRequired ()
{
    SYS_DEBUG ("");

    m_DbusIf->callWithCallback (
            QString ("batteryBarValue"), QList<QVariant> (), this,
            SIGNAL (batteryBarValueReceived (int)),
            SLOT (DBusMessagingFailure (QDBusError)));
}

void
BatteryDBusInterface::setPSMThresholdValue (const QString &value)
{
    SYS_DEBUG ("");

    m_DbusIf->call (QDBus::NoBlock, QString ("setPSMThresholdValue"), value);
}

void
BatteryDBusInterface::setPSMValue (
        bool toggle)
{
    SYS_DEBUG ("");

    m_DbusIf->call (QDBus::NoBlock, QString ("setPSMValue"), toggle);

    QDBusError error = m_DbusIf->lastError();
    if (error.isValid()) {
        SYS_WARNING ("%s: %s", SYS_STR (error.name()), 
                SYS_STR (error.message()));
    }
}

void
BatteryDBusInterface::setPSMAutoValue (bool toggle)
{
    SYS_DEBUG ("");

    m_DbusIf->call (QDBus::NoBlock, QString ("setPSMAutoValue"), toggle);
}

void
BatteryDBusInterface::querySent()
{
    SYS_DEBUG ("");
}

void
BatteryDBusInterface::DBusMessagingFailure (
		QDBusError error)
{
    SYS_WARNING ("%s: %s", SYS_STR (error.name()), SYS_STR (error.message()));
    qWarning () << 
	    "BatteryDBusInterface::DBusMessagingFailure()" <<
	    error.name() <<
	    ": " <<
	    error.message();
}

