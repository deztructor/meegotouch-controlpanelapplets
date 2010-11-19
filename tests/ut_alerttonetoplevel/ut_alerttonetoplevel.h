/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#ifndef UT_ALERTTONETOPLEVEL_H
#define UT_ALERTTONETOPLEVEL_H

#include <QtTest/QtTest>
#include <QObject>

class Ut_AlertToneToplevelTests: public QObject
{
Q_OBJECT
private slots:
      void init();
      void cleanup();
      void initTestCase(){};
      void cleanupTestCase(){};
      void alerttonetoplevelSetTitle ();
      void alerttonetoplevelSetViewMenuItems ();

private:
private slots:
};

#endif
