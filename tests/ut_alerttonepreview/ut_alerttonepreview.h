/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#ifndef UT_ALERTTONEPREVIEW_H
#define UT_ALERTTONEPREVIEW_H

#include <QtTest/QtTest>
#include <QObject>
#include <MApplication>

class Ut_AlertTonePreviewTests : public QObject
{
Q_OBJECT
private slots:
      void init(){};
      void cleanup(){};
      void initTestCase();
      void cleanupTestCase();
      void alerttonepreviewConstructor ();
      void alerttonepreviewfname();
      void alerttonepreviewProfileToGstVolume();
      void alerttonepreviewProfileVolumeChanged();
private:
      MApplication * m_App;
      int m_argc;
      char **m_argv;
private slots:
};

#endif
