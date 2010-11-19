/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#ifndef UT_SOUNDSETTINGSAPPLET_H
#define UT_SOUNDSETTINGSAPPLET_H

#include <QtTest/QtTest>
#include <QObject>
#include <MApplication>

class Ut_SoundSettingsAppletTests : public QObject
{
Q_OBJECT

private slots:
      void init(){};
      void cleanup(){};
      void initTestCase();
      void cleanupTestCase();
      void soundsettingsappletConstructor ();
      void soundsettingsappletconstructWidget ();
      void soundsettingsappletinit();
      void soundsettingsapplettitle();
      void soundsettingsappletviewMenuItems();
      void soundsettingsappletToplevelDestroyed();
private:
      MApplication * m_App;
private slots:
};

#endif
