/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#ifndef UT_BATTERYDBUSINTERFACE_H
#define UT_BATTERYDBUSINTERFACE_H

#include <QtTest/QtTest>
#include <QObject>


class Ut_BatteryDbusInterface : public QObject
{
Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();
};

#endif
