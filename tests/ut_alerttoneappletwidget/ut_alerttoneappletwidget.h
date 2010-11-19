/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#ifndef UT_ALERTTONEAPPLETWIDGET_H
#define UT_ALERTTONEAPPLETWIDGET_H

#include <QtTest/QtTest>
#include <QObject>
#include <MApplication>

class Ut_AlertToneAppletWidgetTests: public QObject
{
Q_OBJECT

public:
      Ut_AlertToneAppletWidgetTests() { m_App = 0; }
private slots:
      void init(){};
      void cleanup(){};
      void initTestCase();
      void cleanupTestCase();
      void alerttoneappletwidgetConstructor();
      void alerttoneappletwidgetRetranslateUi();
      void alerttoneappletwidgetCreateContent();
      void alerttoneappletwidgetCreateAlertTonesList();
      void alerttoneappletwidgetCreateFeedbackList();
private:
      MApplication * m_App;
private slots:
};

#endif
