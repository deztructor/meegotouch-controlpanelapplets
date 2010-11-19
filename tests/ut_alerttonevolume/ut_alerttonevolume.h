/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#ifndef UT_ALERTTONEVOLUME_H
#define UT_ALERTTONEVOLUME_H

#include <QtTest/QtTest>
#include <QObject>
#include <MApplication>


class Ut_AlertToneVolumeTests : public QObject
{
Q_OBJECT

public:
      Ut_AlertToneVolumeTests(){ m_App = 0;}
private slots:
      void init();
      void cleanup();
      void initTestCase(){};
      void cleanupTestCase(){};
      void alerttonevolumeConstructor();
      void alerttonevolumeVolumeChanged_data();
      void alerttonevolumeVolumeChanged();
      void alerttonevolumeFileNameChanged();
      void alerttonevolumeSlider();
      void alerttonevolumeWidgetVolumeChanged();
private:
      MApplication * m_App;
private slots:
};

#endif
