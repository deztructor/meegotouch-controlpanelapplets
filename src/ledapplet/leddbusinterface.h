/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef LEDDBUSINTERFACE_H
#define LEDDBUSINTERFACE_H

#include <QObject>
#include <QDBusError>

class QDBusInterface;

class LedDBusInterface : public QObject
{
    Q_OBJECT

public:
    LedDBusInterface ();
    ~LedDBusInterface ();

    void ledStateRequired ();
    void setLedState (bool enabled);

    void eventsLedStateRequired ();
    void setEventsLedState (bool enabled);

signals:
    void ledStateReceived (bool state);
    void eventsLedStateReceived (int state);

public slots:
    void DBusMessagingFailure (QDBusError error);

private:
    QDBusInterface *m_DbusIf;
};

#endif
