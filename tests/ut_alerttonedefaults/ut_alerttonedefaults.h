/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#ifndef UT_ALERTTONEDEFAULTS_H
#define UT_ALERTTONEDEFAULTS_H

#include <QtTest/QtTest>
#include <QObject>
#include <MApplication>
#include  "alerttonedefaults.h"

class Ut_AlertToneDefaultsTests : public QObject
{
Q_OBJECT
public:
      Ut_AlertToneDefaultsTests(){m_App = 0;}
private slots:
      void init(){};
      void cleanup(){};
      void initTestCase();
      void cleanupTestCase();
      void alerttonedefaultsConstructor();
      void alerttonedefaultsToneChanged();
      void alerttonedefaultsItemClicked();
/*
 * FIXME: AlertToneDefaults::selectedIndex () has been removed.
 */
      //void alerttonedefaultsSelectedIndex ();
      /*
       * FIXME: AlertToneDefaults::selectedFile() has been removed.
       */
	  //void alerttonedefaultsSelectedFile ();  
/*
 * FIXME: AlertToneDefaults::rowCount() has been removed.
 */
      //void alerttonedefaultsRowCount();    
private:
      MApplication *m_App;
      AlertToneDefaults *testATD;      
private slots:
	  void receiveItemClicked(const QString & itemName);
};

#endif
