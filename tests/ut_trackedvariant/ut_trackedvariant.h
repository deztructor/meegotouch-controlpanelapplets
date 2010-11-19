/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#ifndef UT_TRACKEDVARIANT_H
#define UT_TRACKEDVARIANT_H

#include <QtTest/QtTest>
#include <QObject>

class Ut_TrackedVariantTests : public QObject
{
Q_OBJECT

private slots:
      void init();
      void cleanup();
      void initTestCase(){};
      void cleanupTestCase(){};
      void trackedvariantConstructor();
      void trackedvariantSetValue();
      void trackerSignalEmited();
      void trackedvariantRealSetValue();
private:
      int cnt;
private slots:
      void  catchsignal(){ cnt = 1; }
};

#endif
