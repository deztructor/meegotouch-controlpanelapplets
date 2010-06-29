/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#ifndef UT_ABOUTBUSINESSLOGIC_H
#define UT_ABOUTBUSINESSLOGIC_H

#include <QtTest/QtTest>
#include <QObject>
#include <QPointer>

#include "aboutbusinesslogic.h"

class MApplication;

class Ut_AboutBusinessLogic : public QObject 
{
Q_OBJECT

private slots:
    void init ();
    void cleanup ();
    void initTestCase ();
    void cleanupTestCase ();

    void testImei ();
    void testOsName ();
    void testOsVersion ();
    void testBluetooth ();
private:
    MApplication                  *m_App;
    QPointer <AboutBusinessLogic>  m_Api;
};

#endif
