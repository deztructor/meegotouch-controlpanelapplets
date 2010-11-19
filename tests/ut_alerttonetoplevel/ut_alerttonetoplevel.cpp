/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et sw=4 ts=4 sts=4: */

#include <MApplication>
#include <QString>
#include <QStringList>
#include "alerttonetoplevel.h"
#include "ut_alerttonetoplevel.h"
#include <QtTracker/Tracker>
#ifdef HAVE_LIBPROFILE
#include <profiled/libprofile.h>
#endif
#include <QDebug>

/******************************************************************************
 * Ut_AlertToneToplevel implementation.
 */
void
Ut_AlertToneToplevelTests::init()
{
}

void
Ut_AlertToneToplevelTests::cleanup()
{
}

void
Ut_AlertToneToplevelTests::alerttonetoplevelSetTitle ()
{
      AlertToneToplevel  attl(0);
      attl.setTitle( "title" );
      QCOMPARE  ( QString ("title"), attl.title() );
}

#define NUM  3
void
Ut_AlertToneToplevelTests::alerttonetoplevelSetViewMenuItems ()
{
      AlertToneToplevel  attl(0);
      QVector<MAction *>   mav;
      for (unsigned int  i =0 ; i < NUM ; i++)
      {
            mav.push_back (new MAction (0) );
      }
      attl.setViewMenuItems(mav);
      QCOMPARE ( NUM, attl.viewMenuItems().size() );
}
MApplication *app;

QTEST_APPLESS_MAIN(Ut_AlertToneToplevelTests)
