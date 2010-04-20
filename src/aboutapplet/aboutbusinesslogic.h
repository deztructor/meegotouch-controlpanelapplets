/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#ifndef ABOUTBUSINESSLOGIC_H
#define ABOUTBUSINESSLOGIC_H

#include <QObject>

class QString;

class AboutBusinessLogic : public QObject
{
    Q_OBJECT

public:
    AboutBusinessLogic ();
    ~AboutBusinessLogic ();

    QString osVersion ();
    QString WiFiAddress ();
    QString BluetoothAddress ();
    QString IMEI ();
};

#endif
