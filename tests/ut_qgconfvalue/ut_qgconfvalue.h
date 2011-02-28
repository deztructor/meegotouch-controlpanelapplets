/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef UT_QGCONFVALUE_H
#define UT_QGCONFVALUE_H

#include <QObject>
#include <QtTest/QtTest>
#include <MApplication>

/*!
 * FIXME: It seems that this test checks only the setting of the values, it does
 * not sheck the reading of the values.
 */
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
