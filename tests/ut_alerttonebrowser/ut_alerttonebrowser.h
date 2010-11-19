/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#ifndef UT_ALERTTONEBROWSER_H
#define UT_ALERTTONEBROWSER_H

#include <QtTest/QtTest>
#include <QObject>
#include <MApplication>
#include "alerttonebrowser.h"

class Ut_AlertToneBrowserTests : public QObject
{
Q_OBJECT

public:
      Ut_AlertToneBrowserTests() { m_App = 0;}
private slots:

      void init();
      void cleanup();
      void initTestCase();
      void cleanupTestCase();
      void alerttonebrowserConstructor();
      void alerttonebrowserRetranslateUi();
      /*
       * FIXME: This test causes segmentation fault, the contentpicker should be
       * stubbed.
       */
      //void alerttonebrowserLaunchMusicBrowser();
/*
 * The hideShowSelectCurrent() method is obsolete, use the 
 *AlertToneDefaults::selectAndScroll() method instead.
 */
      //void alerttonebrowserHideShowSelectCurrent();
      void alerttonebrowserSetAlertTone();
      void alerttonebrowserCurrentClicked();
      void alerttonebrowserAccept();
private:
      MApplication * m_App;
      AlertTone *  at;
      int m_argc;
      char **m_argv;
      QString savedAlertTone;
private slots:
};

#endif
