/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#ifndef Ut_LedDbusInterface_H
#define Ut_LedDbusInterface_H

#include <QtTest/QtTest>
#include <QObject>

class QString;
class QStringList;
class LedDBusInterface;

class Ut_LedDbusInterfacePrivate : public QObject 
{
Q_OBJECT

public:
    Ut_LedDbusInterfacePrivate ();

public slots:
    void ledStateReceived (bool state);
    void eventsLedStateReceived (bool state);

public:
    bool m_ledStateReceived;
    bool m_eventsLedStateReceived;
};

class Ut_LedDbusInterface : public QObject
{
Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testLedStatesArrived ();
private:
    bool waitforit (const QString &name, bool *what);

private:
    Ut_LedDbusInterfacePrivate *m_priv;
    LedDBusInterface *m_LedDBusInterface;
};

#endif
