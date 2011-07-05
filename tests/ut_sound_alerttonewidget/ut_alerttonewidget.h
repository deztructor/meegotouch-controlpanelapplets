/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#ifndef UT_ALERTTONEWIDGET_H
#define UT_ALERTTONEWIDGET_H

#include <QtTest/QtTest>
#include <QObject>
#include <MApplication>

class Ut_AlertToneWidgetTests : public QObject
{
Q_OBJECT
public:
      Ut_AlertToneWidgetTests(){m_App = 0;}
private slots:
      void init(){};
      void cleanup(){};
      void initTestCase();
      void cleanupTestCase();
      void alerttonewidgetConstructor ();
      void alerttonewidgetAlertToneChanged();
      void alerttonewidgetRetranslateUi();
      void alerttonewidgetClicked();
private:
      MApplication * m_App;
      int  signalFlag;
protected slots:
      void  receiveSignal(int){signalFlag = 1;}
};

#endif
