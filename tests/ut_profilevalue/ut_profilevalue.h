/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#ifndef UT_PROFILEVALUE_H
#define UT_PROFILEVALUE_H

#include <QtTest/QtTest>
#include <QObject>
#include <MApplication>

class Ut_ProfileValueTests: public QObject
{
Q_OBJECT

private slots:
      void init();
      void cleanup();
      void initTestCase();
      void cleanupTestCase();

      void profilevaluePossibleValues_data();
      void profilevaluePossibleValues();
      void profilevaluefetchFromBackend_data();
      void profilevaluefetchFromBackend();
      void profilevalueGetType_data();
      void profilevalueGetType();
      void profilevalueNotifyValue();
private:
      MApplication *m_App;
      int           cnt;

private slots:
      void  catchsignal(){ cnt = 1; qDebug() << "CATCH \n\n"; }
};

#endif
