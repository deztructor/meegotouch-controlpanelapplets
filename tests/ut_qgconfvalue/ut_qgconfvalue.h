/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#ifndef UT_QGCONFVALUE_H
#define UT_QGCONFVALUE_H

#include <QObject>
#include <QtTest/QtTest>
#include <MApplication>

class Ut_QGConfValueTests : public QObject
{
Q_OBJECT

public:
      Ut_QGConfValueTests (){ m_App = 0;}

private slots:
    void init(){};
    void cleanup(){};
    void initTestCase();
    void cleanupTestCase();
    void qgconfvalueConstructor ();
    void qgconfvalueNotifyValue ();
    void qgconfvalueRealSetValue_data();
	void qgconfvalueRealSetValue();

private:
      MApplication  *m_App;
      bool           m_SignalFlag;

private slots:
    // FIXME: This still counts as a test case!
    void  testIfNotify ();
};

#endif
