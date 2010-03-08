/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#ifndef UT_BATTERYDBUSINTERFACE_H
#define UT_BATTERYDBUSINTERFACE_H

#include <QtTest/QtTest>
#include <QObject>

class QString;
class QStringList;
class BatteryDBusInterface;

class Ut_BatteryDbusInterfacePrivate : public QObject {
Q_OBJECT

public:
    Ut_BatteryDbusInterfacePrivate ();
    
public slots:
    void PSMValueReceived (bool PSMValue);
    void PSMAutoValueReceived (bool PSMAutoValue);
    void PSMThresholdValueReceived (QString);
    void PSMThresholdValuesReceived (QStringList ThresholdValues);

public:
    bool m_PSMValueArrived;
    bool m_PSMValue;
    
    bool m_PSMAutoValueArrived;
    bool m_PSMAutoValue;
    
    bool m_ThresholdValueArrived;
    QString m_ThresholdValue;

    bool m_ThresholdValuesArrived;
    QStringList m_ThresholdValues;
};

class Ut_BatteryDbusInterface : public QObject
{
Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testGetPSMValue ();
    void testGetPSMAutoValue ();
    void testGetThresholdValue ();
    void testGetThresholdValues ();

private:
    bool waitforit (const QString &name, bool *what);

private:
    Ut_BatteryDbusInterfacePrivate *m_priv;
    BatteryDBusInterface *m_BatteryDBusInterface;
};

#endif
