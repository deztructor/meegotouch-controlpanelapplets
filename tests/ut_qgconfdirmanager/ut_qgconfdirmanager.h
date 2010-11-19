/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#ifndef UT_QCONFDIRMANAGER_H
#define UT_QCONFDIRMANAGER_H

#include <QtTest/QtTest>
#include <QObject>
#include <MApplication>

class Ut_QConfDirManager: public QObject
{
Q_OBJECT

private slots:
      void init(){};
      void cleanup(){};
      void initTestCase();
      void cleanupTestCase();
      void qconfdirmanagerGConfListenNode ();
      void qconfdirmanagerAddDir();
      void qconfdirmanagerRmDir();
      void qconfdirmanagerPrune();
	  void qconfdirmanagerUnsetListening();
private:
      MApplication *  m_App;
private slots:
};

#endif
