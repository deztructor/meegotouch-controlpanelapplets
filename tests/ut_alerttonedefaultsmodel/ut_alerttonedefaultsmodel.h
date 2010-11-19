/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#ifndef UT_ALERTTONEDEFAULTSMODEL_H
#define UT_ALERTTONEDEFAULTSMODEL_H

#include <QtTest/QtTest>
#include <QObject>
#include <MApplication>

class AlertToneDefaultsModel;

class Ut_AlertToneDefaultsModelTests : public QObject
{
Q_OBJECT
public:
      Ut_AlertToneDefaultsModelTests():filesCount1(10),filesCount2(4){m_App = 0;}
private slots:
      void init(){};
      void cleanup(){};
      void initTestCase();
      void cleanupTestCase();
      void alerttonedefaultsmodelAddSingleItem();
private:
      void  removeFiles (unsigned  int num,unsigned  int asciBase = 65, QString apendix = ".aac", QString directory = "");
      void  createFiles (unsigned  int num,unsigned  int asciBase = 65, QString apendix = ".aac", QString directory = "");
      MApplication * m_App;
      const int   filesCount1;
      const int   filesCount2;
private:
      void clearData (AlertToneDefaultsModel &atdm);
};

#endif
